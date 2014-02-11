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

#include "mfiles/mfilesconstants.h"

ClassCache::ClassCache( VaultCore* parent ) :
	StructureCacheBase( "/structure/classes", parent )
{

}

//! Fetches the classes of the specified object type.
QList< int > ClassCache::classesOfObjectType( int objectType )
{
	// Lock.
	QMutexLocker lock( &m_mutex );

	// Should not request for classes of all object types via this method.
	Q_ASSERT( objectType != MFiles::Constants::AllObjectTypes );

	// Return the classes of the given object type.
	QList< int > classes = m_classesByObjectType.values( objectType );
	return classes;
}

//! Override this to clear the satellite data when the cache contents is cleared.
void ClassCache::clearSatelliteDataNts()
{
	// Clear.
	m_classesByObjectType.clear();
}

//! Override this to populate satellite data that after the cache contens has been refreshed.
void ClassCache::populateSatelliteDataNts()
{
	// Construct the classes of object type mapping.
	const QJsonArray& data = this->dataNts();
	for( QJsonArray::const_iterator itr = data.constBegin(); itr != data.constEnd(); itr++ )
	{
		QJsonObject asObject = (*itr).toObject();
		int id = asObject[ "ID" ].toDouble();
		int objectType= asObject[ "ObjType" ].toDouble();
		m_classesByObjectType.insert( objectType, id );

	}  // end foreach
}
