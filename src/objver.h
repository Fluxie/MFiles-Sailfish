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

#ifndef OBJVER_H
#define OBJVER_H

#include <QJsonObject>

#include "objid.h"

class ObjVer
{
// Public interface.
public:

	//! Initializes new ObjVer object from the given Json object.
	ObjVer(
		const QJsonObject& json
	);

	//! Initializes new ObjVer object from the given Json object.
	ObjVer(
		const ObjID& id,
		int version
	);

	//! Object identitys as ObjID.
	const ObjID& objId() const { return m_id; }

	//! Object type.
	int type() const { return m_id.type(); }

	//! Object id.
	int id() const { return m_id.id(); }

	//! Object version.
	int version() const { return m_version; }

	//! Converts this ObjVer to Json object.
	QJsonObject toJsonObject() const;

	//! Less-than operator.
	bool operator<(
		const ObjVer& rightSide
	) const;

	//! Equality comparison.
	bool operator==(
		const ObjVer& rightSide
	) const;

// Private data.
private:

	ObjID m_id;  //!< Object id.
	int m_version;  //!< Object version.
};

//! Hash function impelemntation for ObjVer
inline unsigned int qHash(const ObjVer &key, unsigned int seed)
{
	return key.type() ^ seed ^ key.id() ^ key.version();
}


#endif // OBJVER_H
