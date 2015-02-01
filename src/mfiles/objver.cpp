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

#include "objver.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

//! Initializes new ObjVer object from the given Json object.
ObjVer::ObjVer(
	const QJsonObject& json
)
{
	MFilesTypeWrapper wrap( __FILE__, json );
	m_type = wrap[ "Type" ].toDouble();
	m_id = wrap[ "ID" ].toDouble();
	m_version = wrap[ "Version" ].toDouble();
}

ObjVer::ObjVer( const QJsonValue& value )
{
	MFilesTypeWrapper wrap( __FILE__, value );
	m_type = wrap[ "Type" ].toDouble();
	m_id = wrap[ "ID" ].toDouble();
	m_version = wrap[ "Version" ].toDouble();
}

//! Initializes new ObjVer object from the given Json object.
ObjVer::ObjVer(
	const ObjID& id,
	int version
) :
	m_type( id.type() ),
	m_id( id.id() ),
	m_version( version )
{	
}

//! Object identitys as ObjID.
ObjID ObjVer::objId() const
{
	// Return as objid.
	return ObjID( this->type(), this->id() );
}

//! Converts this ObjVer to Json object.
QJsonObject ObjVer::toJsonObject() const
{
	// Construct ObjVer Json object.
	QJsonObject objver;
	objver[ "Type" ] = this->type();
	objver[ "ID" ] = this->id();
	objver[ "Version" ] = this->version();
	return objver;
}

/*!
 *Less-than operator.
 */
bool ObjVer::operator<(
	const ObjVer& rightSide
) const
{
	// ObjID is '<'?
	if( this->objId() < rightSide.objId() )
		return true;
	else if( rightSide.objId() < this->objId() )
		return false;

	// Version is '<'?
	if( this->version() < rightSide.version() )
		return true;
	else if( this->version() > rightSide.version() )
		return false;

	// Equal.
	return false;
}

/*!
 *Equality comparison.
 */
bool ObjVer::operator==(
	const ObjVer& rightSide
) const
{
	// Check for equality.
	bool equal = ( ! ( ( *this ) < rightSide ) ) &&
					( ! ( rightSide < ( *this ) ) );
	return equal;
}

}
