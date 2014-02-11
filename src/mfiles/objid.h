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

#ifndef OBJID_H
#define OBJID_H

#include <QJsonObject>

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

//! C++ representation of ObjID.
class ObjID
{
public:

	//! Initializes new ObjID object.
	ObjID( int type, int id );

	//! Initializes new ObjID object from the given Json object.
	ObjID(
		const QJsonObject& json
	);

	//! Object type.
	int type() const { return m_type; }

	//! Object id.
	int id() const { return m_id; }

	//! Converts this ObjID to Json object.
	QJsonObject toJsonObject() const;

	//! Less-than operator.
	bool operator<(
		const ObjID& rightSide
	) const;

	//! Equality comparison.
	bool operator==(
		const ObjID& rightSide
	) const;

	//! Inequality comparison.
	bool operator!=(
		const ObjID& rightSide
	) const
	{ return ! ( (*this) == rightSide ); }

// Private data.
private:

	int m_type;  //!< Object type.
	int m_id;  //!< Object id.
};

//! Hash function impelemntation for ObjVer
inline unsigned int qHash(const ObjID &key, unsigned int seed)
{
	return seed ^ key.type() ^ key.id();
}

//! Hash function impelemntation for ObjVer
inline unsigned int qHash( const ObjID &key )
{
	return key.type() ^ key.id();
}


}


#endif // OBJID_H
