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

#ifndef OBJECTCACHE_H
#define OBJECTCACHE_H

#include <QObject>
#include <QMutex>
#include <QCache>
#include <QSharedPointer>

#include "../mfiles/objid.h"
#include "objectcore.h"

// Forward declarations.
class VaultCore;

/**
 * @brief Cache for objects and related content retrieved from the server.
 */
class ObjectCache : public QObject
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new object cache.
	 * @param parent Parent for the cache.
	 */
	explicit ObjectCache( VaultCore* parent );
	
signals:
	
public slots:

	/**
	 * @brief Establishes new object core for the given object version.
	 * @param id Object id.
	 * @return ObjectCore representing the M-Files object.
	 */
	QSharedPointer< ObjectCore > object(
		const MFiles::ObjID& id
	);
	
// Private data.
private:

	VaultCore* m_vault;

	mutable QMutex m_lock;
	QCache< MFiles::ObjID, QSharedPointer< ObjectCore > > m_cache;  //!< Cache of objects.
};

#endif // OBJECTCACHE_H
