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

#ifndef CLASSCACHE_H
#define CLASSCACHE_H

#include <QList>
#include <QJsonArray>
#include <QMultiHash>
#include <QObject>

#include "structurecachebase.h"

// Forward declarations.
class VaultCore;

/**
 * @brief Cache for object classes retrieved from the server.
 */
class ClassCache : public StructureCacheBase
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new ClassCache object.
	 * @param parent PA
	 */
	explicit ClassCache( VaultCore* parent = 0);

	/**
	 * @brief Uninitializes this object.
	 */
	virtual ~ClassCache() {}

	/**
	 * @brief Gets the classes that belong to specified object type.
	 * @param objectType Classes of this object type are returned.
	 * @return Classes of the specified object type.
	 */
	QList< int > classesOfObjectType( int objectType );
	
signals:
	
public slots:


protected:

	/**
	 * @brief Clears the satellite data controlled by this cache.
	 */
	virtual void clearSatelliteDataNts();

	/**
	 * @brief Updates the satellite data controlled by this cache when the cache is refreshed.
	 */
	virtual void populateSatelliteDataNts();

// Private data.
private:

	QMultiHash< int, int > m_classesByObjectType;  //!< Classes of the object type.

};

#endif // CLASSCACHE_H
