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

#ifndef OBJECTTYPECACHE_H
#define OBJECTTYPECACHE_H

#include <QObject>

#include "structurecachebase.h"
#include "valuelistkey.h"

// Forward declarations.
class TypedValueFilter;
class VaultCore;
class ValueListCore;

/*!
 * \brief The ObjectTypeCache class
 *
 * Container for object types.
 */
class ObjectTypeCache : public StructureCacheBase
{


	//! Definition for cached value lists
	typedef QHash< ValueListKey, ValueListCore* >  VALUELISTS;

	Q_OBJECT
public:

	//! Constructor.
	explicit ObjectTypeCache( VaultCore *parent = 0 );

	//! Destructor.
	virtual ~ObjectTypeCache() {}

	//! Gets value list.
	ValueListCore* list( int id ) const;

	//! Gets value list.
	ValueListCore* list( int id, const TypedValueFilter* filter );

signals:

	//! Signaled when a value list core has become available.
	void valueListAvailable( ValueListCore* core, ObjectTypeCache* source );

public slots:



// Protected interface.
protected:

	//! Override this to clear the satellite data when the cache contents is cleared.
	virtual void clearSatelliteDataNts();

	//! Override this to populate satellite data that after the cache contens has been refreshed.
	virtual void populateSatelliteDataNts();

// Private interface.
private:

	//! Requests value list.
	ValueListCore* getNewValueListNts( int id, int owner, const TypedValueFilter* filter );

// Private data.
private:

	VALUELISTS m_valueLists;  // Cached value listss

};

#endif // OBJECTTYPECACHE_H
