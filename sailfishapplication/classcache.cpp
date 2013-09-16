/*
 *  Copyright 2013 Juha Lepola
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

#include "classcache.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkReply>

#include "vaultcore.h"
#include "mfwsrest.h"

ClassCache::ClassCache(VaultCore *parent) :
	QObject(parent),
	m_vault( parent ),
	m_populated( false )
{
	// Create the rest object.
	m_rest = new MfwsRest( m_vault->url(), this );
	QObject::connect( m_vault, &VaultCore::authenticationChanged, m_rest, &MfwsRest::setAuthentication );

	// Connect missing and try fetch.
	QObject::connect( this, &ClassCache::missing, this, &ClassCache::tryFetch, Qt::QueuedConnection );
}

/*!
 *Gets class from the cache.
 */
QJsonValue ClassCache::get( int id ) const
{
	QMutexLocker lock( &m_lock );

	// Try searchin
	CLASS_CONTAINER::const_iterator itr = m_classes.find( id );
	if( itr != m_classes.end() )
	{
		// Return the value.
		return itr.value();
	}
	else
	{
		// Emit missing.
		emit missing( id );

		// Not found.
		return QJsonValue::Null;
	}
}

/**
 *Attempts to cache the specified class.
 */
void ClassCache::tryFetch( int id )
{

}

//! Populates the cache.
void ClassCache::requestRefresh()
{
	// Request the refresh.	
	QNetworkReply* reply = m_rest->getJson( "/structure/classes" );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		this->setContentFrom( reply ); } );
}

//! Sets the cache content from the given network reply.
void ClassCache::setContentFrom( QNetworkReply* reply )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	QJsonArray results = result.array();

	// Populate the cache.
	{
		QMutexLocker lock( &m_lock );

		// Clear previous data.
		m_classes.clear();
		m_populated = false;

		// Store the JSON objects that represents classes.
		for( QJsonArray::iterator itr = results.begin(); itr != results.end(); itr++ )
		{
			// Store the class.
			QJsonObject mfilesClass = (*itr).toObject();
			int id = mfilesClass[ "ID" ].toDouble();
			m_classes.insert( id, mfilesClass );

		}  // end for
		m_populated = true;

	}  // end lock

	// The list has been refreshed.
	emit refreshed();
}
