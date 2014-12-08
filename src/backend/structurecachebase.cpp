/*
*  This file is part of M-Files for Sailfish.
*
*  M-Files for Sailfish is free software: you can redistribute it
*  and/or modify it under the terms of the GNU General Public
*  License as published by the Free Software Foundation, either
*  version 3 of the License, or (at your option) any later version.
*
*  M-Files for Sailfish is distributed in the hope that it will be
*  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
*  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with M-Files for Sailfish. If not, see
*  <http://www.gnu.org/licenses/>.
*/

#include "structurecachebase.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QMutexLocker>
#include <QStringList>
#include <QByteArray>
#include <QNetworkReply>

#include "asyncfetch.h"
#include "vaultcore.h"
#include "../mfwsrest.h"

StructureCacheBase::StructureCacheBase(
	const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
	VaultCore* parent,  //!< Parent vault.
	bool immediateRefresh/* = true*/  //!< True if the cache should be refreshed immediately.
) :
	CoreBase( parent, parent ),	
	m_resource( resource ),
	m_populated( false ),
	m_nextCookie( 0 )
{
	// This object might have been already transferred to other thread than the one where it has been created.
	if( immediateRefresh )
		QMetaObject::invokeMethod( this, "requestRefresh", Qt::AutoConnection );
}

//! Constructor.
StructureCacheBase::StructureCacheBase(
	const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
	const QString& fetchOne,  //!< String format for fetching one item.
	VaultCore* parent,  //!< Parent vault.
	bool immediateRefresh/* = true*/  //!< True if the cache should be refreshed immediately.
) :
	CoreBase( parent, parent ),
	m_resource( resource ),
	m_fetchOne( fetchOne ),
	m_populated( false ),
	m_nextCookie( 0 )
{
	// This object might have been already transferred to other thread than the one where it has been created.
	if( immediateRefresh )
		QMetaObject::invokeMethod( this, "requestRefresh", Qt::AutoConnection );
}

/*!
 * Gets item from the cache.
 *
 */
AsyncFetch* StructureCacheBase::get( int id ) const
{
	QMutexLocker lock( &m_mutex );

	// Construct the object for fetching.
	int cookie = getNextCookieNts();
	AsyncFetch* fetch = new AsyncFetch( cookie );

	// Try searching for the requested item.
	CACHE_MAPPER::const_iterator itr = m_cache.find( id );
	if( itr != m_cache.end() )
	{
		// Found it, return the value.		
		QJsonValue value = m_data[ itr.value() ];
		fetch->itemFetched( cookie, value );
	}
	else if( ! m_fetchOne.isEmpty() )
	{
		// Make the necessary connections so that we wil
		QObject::connect( this, &StructureCacheBase::itemFetched, fetch, &AsyncFetch::itemFetched );

		// Make the fetch request.
		QMetaObject::invokeMethod( const_cast< StructureCacheBase* >( this ), "fetchOneItem", Q_ARG( int, cookie ), Q_ARG( int, id ) );
	}
	else
	{
		// We cannot fetch the item because we do not know how.
		fetch->reportError( cookie );

	} // end if

	// Return fetch descriptor.
	return fetch;
}

AsyncFetch* StructureCacheBase::list() const
{
	QMutexLocker lock( &m_mutex );

	// Construct the object for fetching.
	int cookie = getNextCookieNts();
	AsyncFetch* fetch = new AsyncFetch( cookie );

	// Return items from the cache if we are populated
	// Otherwise star refresh.
	if( this->populated() )
	{
		// Found it, return the value.
		fetch->itemsFetched( cookie, m_data );
	}
	else
	{
		// Make the necessary connections so that AsyncFetch receives the items.
		QObject::connect( this, &StructureCacheBase::itemsFetched, fetch, &AsyncFetch::itemsFetched );

		// Make the refresh request.
		QMetaObject::invokeMethod( const_cast< StructureCacheBase* >( this ), "requestRefreshWithCookie", Q_ARG( int, cookie ) );
	}

	// Return fetch descriptor.
	return fetch;
}

//! Populates the cache.
void StructureCacheBase::requestRefresh()
{
	// Get cookie.
	int cookie;
	{
		QMutexLocker lock( &m_mutex );

		cookie = this->getNextCookieNts();
	}

	// Delegate.
	this->requestRefreshWithCookie( cookie );
}

//! Sets the cache content from the given network reply.
void StructureCacheBase::setContentFrom( int cookie, QNetworkReply* reply )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	if( reply->error() != QNetworkReply::NoError )
		return;  // We are using the automatic reporting of MfwsRest object.

	// Populate the cache.
	bool populatedStatusChanged = false;
	QJsonArray copyOfData;
	{
		QMutexLocker lock( &m_mutex );

		// Clear previous data.
		clearSatelliteDataNts();
		m_cache.clear();
		bool wasPopulated = m_populated;
		m_populated = false;

		// Parse the results.
		// TODO: Allow subclass to define custom parsing.
		if( result.isObject() )
		{
			// The result is an object. Is it formatted as { Items, MoreResults }?
			QJsonObject asObject = result.object();
			QJsonValue asValue = asObject[ "Items" ];
			if( asValue.isArray() )
			{
				m_data = asValue.toArray();
			}
			else
				qCritical( QString( "Unable to parse results. Raw dump %1." ).arg( asObject.keys().join( '; ' ) ).toLatin1() );
		}
		else if( result.isArray() )
		{
			// The resulting data is an array.
			m_data = result.array();
		}
		else
		{
			// Error.
			qCritical( QString( "Unable to parse results. Raw dump %1." ).arg( result.toVariant().toString() ).toLatin1() );

		}  // end if.

		// Post-process the data.
		for( int i = 0; i < m_data.size(); i++ )
		{
			// Normalize the value.
			m_data[ i ] = this->normalizeValue( m_data[ i ] );

			// Make ID -> value mapping.
			const QJsonValue& item = m_data[ i ];
			int id = item.toObject()[ "ID" ].toDouble();
			m_cache.insert( id, i );

		}  // end for
		copyOfData = m_data;
		qDebug( QString( "Populated cache with %1 items." ).arg( m_data.size() ).toLatin1() );

		// Populate satellite data.
		populateSatelliteDataNts();
		m_populated = true;
		if( wasPopulated != m_populated )
			populatedStatusChanged = true;

	}  // end lock

	// The list has been refreshed.
	if( populatedStatusChanged )
		emit populatedChanged();
	emit refreshed();
	emit itemsFetched( cookie, copyOfData );  // TODO: Process error.
}

//! Sets the cache content from the given network reply.
void StructureCacheBase::fetchOneItem( int cookie, int id )
{
	qDebug( "fetchOneItem" );

	// Request the retrieval of the item.
	QString fetchResource =  m_fetchOne.arg( id );
	QNetworkReply* reply = this->rest()->getJson( fetchResource );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {

		// Parse the returned JSON.
		QByteArray replyContent = reply->readAll();
		QJsonDocument result = QJsonDocument::fromJson( replyContent );
		qDebug( "fetchOneItem Reply" );
		if( reply->error() != QNetworkReply::NoError )
		{
			qDebug( reply->errorString().toLatin1() );
			return;  // We are using the automatic reporting of MfwsRest object. TODO.
		}

		// Parse the value from the result and inform that fetching the item has been completed.
		QJsonValue value( result.object() );
		value = this->normalizeValue( value );
		QJsonObject asObject = value.toObject();
		qDebug( QString( "Item %1 fetched." ).arg( asObject[ "ID" ].toDouble() ).toLatin1() );
		emit itemFetched( cookie, value );

	} );

	// TODO: Cache the item.
}

void StructureCacheBase::requestRefreshWithCookie( int cookie )
{
	// Request the refresh.
	QNetworkReply* reply = this->rest()->getJson( m_resource );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		this->setContentFrom( cookie, reply ); } );
}
