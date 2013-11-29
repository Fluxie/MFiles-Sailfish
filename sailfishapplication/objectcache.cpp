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

#include "objectcache.h"

#include <QMutexLocker>

#include "vaultcore.h"

ObjectCache::ObjectCache( VaultCore* parent ) :
	QObject(parent),
	m_vault( parent ),
	m_lock( QMutex::NonRecursive ),
	m_cache( 20 )  // Store 20 objects.
{
}

//! Establishes new object core for the given object version.
QSharedPointer< ObjectCore > ObjectCache::object(
	const ObjID& id
)
{
	// Protect access.
	QMutexLocker lock( &m_lock );

	// Return the cached object if it already exists.
	QSharedPointer< ObjectCore >* cachedObject = m_cache.object( id );
	if( cachedObject != 0 )
		return *cachedObject;

	// Create a new ObjectCore for the object version as one did not exist before.
	ObjectCore* object = new ObjectCore( m_vault, id );
	cachedObject = new QSharedPointer< ObjectCore >( object );
	m_cache.insert( id, cachedObject );
	return *cachedObject;
}
