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

//! Cache for classes.
class ClassCache : public StructureCacheBase
{
	Q_OBJECT
public:

	//! Constructor
	explicit ClassCache( VaultCore* parent = 0);

	//! Destructor.
	virtual ~ClassCache() {}

	//! Fetches the classes of the specified object type.
	QList< int > classesOfObjectType( int objectType );
	
signals:
	
public slots:

// Protected interface.
protected:

	//! Override this to clear the satellite data when the cache contents is cleared.
	virtual void clearSatelliteDataNts();

	//! Override this to populate satellite data that after the cache contens has been refreshed.
	virtual void populateSatelliteDataNts();

// Private data.
private:

	QMultiHash< int, int > m_classesByObjectType;  //!< Classes of the object type.

};

#endif // CLASSCACHE_H
