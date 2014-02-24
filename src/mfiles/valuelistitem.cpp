/*
 *  Copyright 2014 Juha Lepola
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

#include "valuelistitem.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

ValueListItem::ValueListItem( const QJsonValue& valueListItem ) :
	MFilesTypeWrapper( valueListItem.toObject() )
{
}

QJsonValue ValueListItem::toLookup() const
{
	// Construct Json object that looks like a lookup.
	Q_ASSERT( ! this->name().isEmpty() );
	QJsonObject lookup;
	lookup[ "Item" ] = this->id();
	lookup[ "DisplayValue" ] = this->name();
	lookup[ "Deleted" ] = false;
	lookup[ "Hidden" ] = false;
	lookup[ "Version" ] = -1;

	// Return the lookup.
	return QJsonValue( lookup );
}

}
