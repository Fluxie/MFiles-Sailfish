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

#include "valuelistcore.h"

#include "vaultcore.h"

ValueListCore::ValueListCore(VaultCore* vault, int valueList ) :
	StructureCacheBase( QString( "/valuelists/%1/items?page" ).arg( valueList ), vault, false ),
	m_valueList( valueList ),
	m_propertyDefinition( -1 )
{
}

//! Constructs value list object to represent one value list accessed via the specified property definition.
ValueListCore::ValueListCore( VaultCore* vault, int valueList, int propertyDefinition ) :
	StructureCacheBase( QString( "/valuelists/%1/items?page&propertydef=%2" ).arg( valueList ).arg( propertyDefinition ), vault, true ),
	m_valueList( valueList ),
	m_propertyDefinition( propertyDefinition )
{

}
