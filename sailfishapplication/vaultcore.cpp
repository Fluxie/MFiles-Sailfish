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

#include "vaultcore.h"

#include "classcache.h"
#include "objectcache.h"
#include "objecttypecache.h"
#include "propertydefcache.h"
#include "valuelistcache.h"

VaultCore::VaultCore(
	const QString& url,
	const QString& authentication,
	QObject *parent ) :
	QObject(parent),
	m_url( url ),
	m_lock( QMutex::NonRecursive ),	
	m_authentication( authentication ),
	m_cachesPopulated( false )
{
	// Metadata caches.
	m_classes = new ClassCache( this );
	m_objectTypes = new ObjectTypeCache( this );
	m_propertyDefinitions = new PropertyDefCache( this );
	m_objectCache = new ObjectCache( this );
	m_valueLists = new ValueListCache( this );

	// Connect authentication info change to refresh events of caches.
	QObject::connect( this, &VaultCore::authenticationChanged, m_classes, &ClassCache::requestRefresh, Qt::QueuedConnection );
	QObject::connect( this, &VaultCore::authenticationChanged, m_objectTypes, &ObjectTypeCache::requestRefresh, Qt::QueuedConnection );
	QObject::connect( this, &VaultCore::authenticationChanged, m_propertyDefinitions, &PropertyDefCache::requestRefresh, Qt::QueuedConnection );
	QObject::connect( this, &VaultCore::authenticationChanged, m_valueLists, &ValueListCache::requestRefresh, Qt::QueuedConnection );
	QObject::connect( m_classes, &ClassCache::refreshed, this, &VaultCore::cacheRefreshed );
	QObject::connect( m_objectTypes, &ObjectTypeCache::refreshed, this, &VaultCore::cacheRefreshed );
	QObject::connect( m_propertyDefinitions, &PropertyDefCache::refreshed, this, &VaultCore::cacheRefreshed );

	qDebug( "VaultCore instantiated." );
}

VaultCore::~VaultCore()
{
	qDebug( "VaultCore destroyed.");
}


//! Authentication.
QString VaultCore::authentication() const
{
	// Protect access.
	QMutexLocker lock( &m_lock );

	// Return the authentication.
	return m_authentication;
}

/*!
 *Updates the authentication info.
 */
void VaultCore::setAuthentication(
	const QString& authentication
)
{
	// Protect access.
	QMutexLocker lock( &m_lock );

	// Update the authentication?
	if( m_authentication != authentication )
	{
		// Authentication info was updated.
		m_authentication = authentication;

		// Emit the changed event.
		emit authenticationChanged( m_authentication );

	}  // end if
}

//! A cache hosted by this core has been refreshed.
void VaultCore::cacheRefreshed()
{
	// Check that all caches have been populated.
	if( ! m_cachesPopulated )
	{
		// Check if all the caches have been populated.
		if( m_classes->populated() &&
				m_objectTypes->populated() &&
				m_propertyDefinitions->populated() )
		{
			// All caches have been populated.
			m_cachesPopulated = true;

			// Emit signal.
			emit allCachesPopulated();
		}
		else
		{
			// One of the caches is till waiting to bee populated.
		}
	}
	else
	{
		// All caches have been populated already.
		// This was a regular refresh.

	}  // end if
}

//! A network error has occurred within the vault.
void VaultCore::networkError( QNetworkReply::NetworkError code, const QString& description )
{
	// Convert to our error object and emit.
	ErrorInfo errorinfo( description );
	emit error( errorinfo );
}
