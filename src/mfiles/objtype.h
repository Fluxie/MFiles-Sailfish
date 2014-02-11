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

#ifndef VALUELIST_H
#define VALUELIST_H

#include "mfilestypecapsule.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

class ObjType : public MFilesTypeCapsule
{
public:

	/**
	 * @brief ValueList
	 */
	ObjType( const QJsonValue& valueList );

	/**
	 * @brief id
	 * @return Id of the value list
	 */
	int id() const { return this->object()[ "ID" ].toDouble(); }

	/**
	 * @brief hasOwner
	 * @return True if this objec type has an owner.
	 */
	bool hasOwner() const { return this->object()[ "HasOwner" ].toBool(); }

	/**
	 * @brief owner
	 * @return The id of the owner type.
	 */
	int owner() const { Q_ASSERT( this->object().contains( "Owner" ) ); return this->object()[ "Owner" ].toDouble(); }
};

}

#endif // VALUELIST_H
