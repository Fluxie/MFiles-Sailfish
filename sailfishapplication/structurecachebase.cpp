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
#include <QByteArray>
#include <QNetworkReply>

#include "vaultcore.h"
#include "mfwsrest.h"

StructureCacheBase::StructureCacheBase(
	const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
	VaultCore* parent  //!< Parent vault.
) :
	QObject( parent ),
	m_vault( parent ),
	m_rest( 0 ),
	m_resource( resource ),
	m_populated( false )
{
	// Create the MfwsRest object for fetching the actual data.
	m_rest = new MfwsRest( m_vault->url(), this );
	QObject::connect( m_vault, &VaultCore::authenticationChanged, m_rest, &MfwsRest::setAuthentication );
}

/*!
 * Gets item from the cache.
 *
 */
QJsonValue StructureCacheBase::get( int id ) const
{
	QMutexLocker lock( &m_lock );

	// Try searching for the requested item.
	CACHE_CONTAINER::const_iterator itr = m_cache.find( id );
	if( itr != m_cache.end() )
	{
		// Found it, return the value.
		return itr.value();
	}
	else
	{
		// Not found, return null Json object.
		return QJsonValue::Null;
	}
}

//! Populates the cache.
void StructureCacheBase::requestRefresh()
{
	// Request the refresh.
	QNetworkReply* reply = m_rest->getJson( m_resource );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		this->setContentFrom( reply ); } );
}

//! Sets the cache content from the given network reply.
void StructureCacheBase::setContentFrom( QNetworkReply* reply )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	QJsonArray results = result.array();

	// Populate the cache.
	{
		QMutexLocker lock( &m_lock );

		// Clear previous data.
		m_cache.clear();
		m_populated = false;

		// Store the JSON objects that represents classes.
		for( QJsonArray::iterator itr = results.begin(); itr != results.end(); itr++ )
		{
			// Store the class.
			QJsonObject item = (*itr).toObject();
			int id = item[ "ID" ].toDouble();
			m_cache.insert( id, item );

		}  // end for
		m_populated = true;

	}  // end lock

	// The list has been refreshed.
	emit refreshed();
}
