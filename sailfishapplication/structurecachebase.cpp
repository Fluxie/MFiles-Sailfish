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

#include "vaultcore.h"
#include "mfwsrest.h"

StructureCacheBase::StructureCacheBase(
	const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
	VaultCore* parent,  //!< Parent vault.
	bool immediateRefresh/* = true*/  //!< True if the cache should be refreshed immediately.
) :
	CoreBase( parent, parent ),	
	m_resource( resource ),
	m_populated( false )
{
	// This object might have been already transferred to other thread than the one where it has been created.
	if( immediateRefresh )
		QMetaObject::invokeMethod( this, "requestRefresh", Qt::AutoConnection );
}

/*!
 * Gets item from the cache.
 *
 */
QJsonValue StructureCacheBase::get( int id ) const
{
	QMutexLocker lock( &m_mutex );

	// Try searching for the requested item.
	CACHE_MAPPER::const_iterator itr = m_cache.find( id );
	if( itr != m_cache.end() )
	{
		// Found it, return the value.
		return m_data[ itr.value() ];
	}
	else
	{
		// Not found, return null Json object.
		return QJsonValue::Null;
	}
}

//! Gets the values as a list.
QJsonArray StructureCacheBase::list() const
{
	QMutexLocker lock( &m_mutex );

	// Return the list.
	return m_data;
}

//! Populates the cache.
void StructureCacheBase::requestRefresh()
{
	// Sanity check.
	if( ! this->isInitialized() )
		qCritical( "StructureCacheBase: requestRefresh without initialization." );

	// Request the refresh.
	QNetworkReply* reply = this->rest()->getJson( m_resource );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		this->setContentFrom( reply ); } );
}

//! Sets the cache content from the given network reply.
void StructureCacheBase::setContentFrom( QNetworkReply* reply )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	if( reply->error() != QNetworkReply::NoError )
		qCritical( QString( "Error while fetching data: %1" ).arg( reply->errorString() ).toLatin1() );

	// Populate the cache.
	bool populatedStatusChanged = false;
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
				m_data = asValue.toArray();
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

		// Store the JSON objects that represents classes.		
		for( int i = 0; i < m_data.size(); i++ )
		{
			// Store the class.
			const QJsonValue& item = m_data[ i ];
			int id = item.toObject()[ "ID" ].toDouble();
			m_cache.insert( id, i );

		}  // end for
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
}
