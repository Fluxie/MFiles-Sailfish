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

#include "vaultcache.h"

#include <QJsonValue>

#include "hostcore.h"
#include "vaultcore.h"
#include "classcache.h"
#include "objecttypecache.h"

VaultCache::VaultCache(QObject *parent) :
	QObject(parent),
	m_core( 0 )
{
}

//! Destructor.
VaultCache::~VaultCache()
{
	// Mark the associated vault core for destruction.
	if( m_core != 0 )
		m_core->deleteLater();
}

/**
 * @brief VaultCache::initialize
 * @param url
 * @param authentication
 */
void VaultCache::initialize(
		const QString& url,
		const QString& authentication )
{
	Q_ASSERT( m_core == 0 );

	// Prepare new core.
	m_core = HostCore::instance()->prepareVault( url );
	m_core->setAuthentication( authentication );

	// Connect events
	QObject::connect( m_core, &VaultCore::allCachesPopulated, this, &VaultCache::allCachesPopulated, Qt::QueuedConnection );
	QObject::connect( m_core->classes(), &ClassCache::refreshed, this, &VaultCache::classesRefreshed, Qt::QueuedConnection );
	QObject::connect( m_core->objectTypes(), &ObjectTypeCache::refreshed, this, &VaultCache::objectTypesRefreshed, Qt::QueuedConnection );
}

//! Gets an item from the cache.
QJsonValue VaultCache::get(
	CacheType type,  //!< The type of the requested item.
	int id
) const
{
	// Try fetching the item.
	QJsonValue item = QJsonValue::Null;
	switch( type )
	{
	case CacheType::Class :
		item = m_core->classes()->get( id );
		break;

	case CacheType::ObjectType :
		item = m_core->objectTypes()->get( id );
		break;

	// Unknown item, return the null we set earlier.
	default:
		break;
	};

	// Return.
	return item;
}
