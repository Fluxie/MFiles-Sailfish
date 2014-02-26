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
 * @brief The ObjectTypeCache class caches all object types fetched from the server.
 *
 */
class ObjectTypeCache : public StructureCacheBase
{
	/**
	 * @brief Definition for cached value lists
	 */
	typedef QHash< ValueListKey, ValueListCore* >  VALUELISTS;

	Q_OBJECT
public:

	/**
	 * @brief Initializes new ObjectTypeCache object.
	 * @param parent Parent for the cache.
	 */
	explicit ObjectTypeCache( VaultCore *parent = 0 );

	//! Destructor.
	virtual ~ObjectTypeCache() {}

	/**
	 * @brief Gets the specified value list from the cache.
	 * @param id The id of the value list
	 * @return Specified value list
	 */
	ValueListCore* list( int id ) const;

	/**
	 * @brief Gets the specified filtered value list.
	 * @param id The if of the value list.
	 * @param filter Filter for filtering the values of the value list.
	 * @return Value list filtered with the given filter.
	 */
	ValueListCore* list( int id, const TypedValueFilter* filter );

signals:

	/**
	 * @brief This signal is emitted when a value list core has become available.
	 * @param core The value list that has become available.
	 * @param source The source of the event.
	 */
	void valueListAvailable( ValueListCore* core, ObjectTypeCache* source );

public slots:



// Protected interface.
protected:

	/**
	 * @brief Clears the satellite data controlled by this cache.
	 */
	virtual void clearSatelliteDataNts();

	/**
	 * @brief Updates the satellite data controlled by this cache when the cache is refreshed.
	 */
	virtual void populateSatelliteDataNts();

	/**
	 * @brief Normalizes the presentation of the value.
	 * @param value The value that is normalized.
	 * @return Normalized value.
	 */
	virtual QJsonValue normalizeValue( QJsonValue value );

// Private interface.
private:

	/**
	 * @brief Gets a new representative for the specified value list.
	 * @param id The id of the value list.
	 * @param owner The id of the owner value list.
	 * @param filter Filter for the new value list.
	 * @return A new representative for the value list.
	 * @remarks { This method is not thread-safe. }
	 */
	ValueListCore* getNewValueListNts( int id, int owner, const TypedValueFilter* filter );

// Private data.
private:

	VALUELISTS m_valueLists;  // Cached value listss

};

#endif // OBJECTTYPECACHE_H
