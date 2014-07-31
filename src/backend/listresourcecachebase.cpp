/*
 *  Copyright 2014 Juha Lepola
 *
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

#include "ListResourceCacheBase.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>
#include <QNetworkReply>
#include <QStringList>

#include "asyncfetch.h"
#include "vaultcore.h"
#include "../mfwsrest.h"

ListResourceCacheBase::ListResourceCacheBase( const QString& resource, VaultCore* vault, VaultCore* parent, bool immediateRefresh ) :
	CoreBase( vault, parent ),
	m_resource( resource ),
	m_populated( false ),
	m_nextCookie( 0 )
{
	// This object might have been already transferred to other thread than the one where it has been created.
	if( immediateRefresh )
		QMetaObject::invokeMethod( this, "requestRefresh", Qt::AutoConnection );
}


bool ListResourceCacheBase::populated() const
{
	QMutexLocker lock( &m_mutex );

	return m_populated;
}

AsyncFetch* ListResourceCacheBase::list() const
{
	QMutexLocker lock( &m_mutex );

	// Construct the object for fetching.
	int cookie = getNextCookieNts();
	AsyncFetch* fetch = new AsyncFetch( cookie );

	// Return items from the cache if we are populated
	// Otherwise star refresh.
	if( this->populatedNts() )
	{
		// Found it, return the value.
		fetch->itemsFetched( cookie, m_data );
	}
	else
	{
		// Make the necessary connections so that AsyncFetch receives the items.
		QObject::connect( this, &ListResourceCacheBase::itemsFetched, fetch, &AsyncFetch::itemsFetched );

		// Make the refresh request.
		QMetaObject::invokeMethod( const_cast< ListResourceCacheBase* >( this ), "requestRefreshWithCookie", Q_ARG( int, cookie ) );
	}

	// Return fetch descriptor.
	return fetch;
}

//! Populates the cache.
void ListResourceCacheBase::requestRefresh()
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
void ListResourceCacheBase::setContentFrom( int cookie, QNetworkReply* reply )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	if( reply->error() != QNetworkReply::NoError )
	{
		qDebug( "Error in fetching content." );
		return;  // We are using the automatic reporting of MfwsRest object.
	}

	// Populate the cache.
	bool populatedStatusChanged = false;
	QJsonArray copyOfData;
	{
		QMutexLocker lock( &m_mutex );

		// Clear previous data.
		/*clearSatelliteDataNts();
		m_cache.clear();*/
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
				copyOfData = m_data;
			}
			else
				qCritical( QString( "Unable to parse results. Raw dump %1." ).arg( asObject.keys().join( '; ' ) ).toLatin1() );
		}
		else if( result.isArray() )
		{
			// The resulting data is an array.
			m_data = result.array();
			copyOfData = m_data;
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

		}  // end for
		qDebug( QString( "Populated cache with %1 items." ).arg( m_data.size() ).toLatin1() );

		// Populate satellite data.
		//populateSatelliteDataNts();
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

void ListResourceCacheBase::requestRefreshWithCookie( int cookie )
{
	// Request the refresh.
	QNetworkReply* reply = this->rest()->getJson( m_resource );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		this->setContentFrom( cookie, reply ); } );
}
