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

#include "objid.h"

ObjID::ObjID( int type, int id ) :
	m_type( type ),
	m_id( id )
{
}

//! Initializes new ObjID object from the given Json object.
ObjID::ObjID(
	const QJsonObject& json
) :
	m_type( (int) json[ "Type" ].toDouble() ),
	m_id( (int) json[ "ID" ].toDouble() )
{

}

//! Converts this ObjID to Json object.
QJsonObject ObjID::toJsonObject() const
{
	// Construct ObjID Json object.
	QJsonObject objid;
	objid[ "Type" ] = this->type();
	objid[ "ID" ] = this->id();
	return objid;
}

/*!
 *Less-than operator.
 */
bool ObjID::operator<(
	const ObjID& rightSide
) const
{
	// Type is '<'?
	if( this->type() < rightSide.type() )
		return true;
	else if( this->type() > rightSide.type() )
		return false;

	// Id is '<'?
	if( this->id() < rightSide.id() )
		return true;
	else if( this->id() > rightSide.id() )
		return false;

	// Equal.
	return false;
}

/*!
 *Equality comparison.
 */
bool ObjID::operator==(
	const ObjID& rightSide
) const
{
	// Check for equality.
	bool equal = ( ! ( ( *this ) < rightSide ) ) &&
					( ! ( rightSide < ( *this ) ) );
	return equal;
}
