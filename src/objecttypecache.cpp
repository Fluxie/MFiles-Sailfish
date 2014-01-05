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

#include "objecttypecache.h"

#include "valuelistcore.h"

ObjectTypeCache::ObjectTypeCache( VaultCore *parent ) :
	StructureCacheBase( "/valuelists", parent )
{
}

//! Gets value list.
ValueListCore* ObjectTypeCache::list( int id ) const
{
	QMutexLocker lock( &m_mutex );

	// Return the value list.
	return m_valueLists[ ValueListKey( id, -1 ) ];
}

//! Gets value list.
ValueListCore* ObjectTypeCache::list( int id, int propertyDefinition )
{
	QMutexLocker lock( &m_mutex );

	// Nothing return if not populated.
    if( ! populated() )
		return 0;

	// Search for the value list cached for this property definition.
	ValueListKey key( id, propertyDefinition );
	VALUELISTS::iterator itr = m_valueLists.find( key );
	if( itr == m_valueLists.end() )
	{
		// The value list for the given property definition was not found.
		// Create a new one.

		// Value list exists at all?
		if( ! m_valueLists.contains( ValueListKey( id, -1 ) ) )
		{
			qDebug( QString( "Requested value list %1 was not found. %2 value lists available." )
					.arg( id ).arg( m_valueLists.size() ).toLatin1() );
			return 0;
		}

		// Return new value list.
		return getNewValueListNts( id, propertyDefinition );
	}
	else
	{
		// Return the value.
		return itr.value();
	}
}

//! Override this to clear the satellite data when the cache contents is cleared.
void ObjectTypeCache::clearSatelliteDataNts()
{
	// Mark each value list for deletion.
	for( VALUELISTS::iterator itr = m_valueLists.begin(); itr != m_valueLists.end(); itr++ )
	{
		itr.value()->deleteLater();
		itr.value()= 0;
	}
	m_valueLists.clear();
}

//! Override this to populate satellite data that after the cache contens has been refreshed.
void ObjectTypeCache::populateSatelliteDataNts()
{
	// Creates value list objects based on the
	const QJsonArray& data = this->dataNts();
	for( QJsonArray::const_iterator itr = data.constBegin(); itr != data.constEnd(); itr++ )
	{
		const QJsonValue& valuelist = *itr;
		int id = valuelist.toObject()[ "ID" ].toDouble();
		ValueListCore* core = new ValueListCore( vault(), id );
		m_valueLists.insert( ValueListKey( id, -1 ), core );

	}  // end for
}

//! Requests value list.
ValueListCore* ObjectTypeCache::getNewValueListNts( int id, int propertyDefinition )
{
	// Instantiate the new value list core for the property definition and return it.
	qDebug( QString( "New value list, id: %1, property definition: %2" ).arg( id ).arg( propertyDefinition ).toLatin1() );	
	ValueListCore* core = new ValueListCore( this->vault(), id, propertyDefinition );
	QObject::connect( core, &ValueListCore::initialized, core, &ValueListCore::requestRefresh );
	m_valueLists.insert( ValueListKey( id, propertyDefinition ), core );

	// Return the core.
	return core;
}
