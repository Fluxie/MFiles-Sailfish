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

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief C++ representation of ObjVer object.
 */
class ObjVer
{
// Public interface.
public:

	/**
	 * @brief Initializes new ObjVer object.
	 * @param json QJsonObject from which the object version is parsed.
	 */
	ObjVer( const QJsonObject& json );

	/**
	 * @brief Initializes new ObjVer object.
	 * @param id ObjID representing object id part.
	 * @param version The version part.
	 */
	ObjVer( const ObjID& id, int version );

	//! Object identitys as ObjID.
	const ObjID& objId() const { return m_id; }

	/**
	 * @brief Gets the object type.
	 * @return The object type.
	 */
	int type() const { return m_id.type(); }

	/**
	 * @brief Gets the object id.
	 * @return The object id.
	 */
	int id() const { return m_id.id(); }

	/**
	 * @brief Gets the object version.
	 * @return  The object version.
	 */
	int version() const { return m_version; }

	/**
	 * @brief Converts this ObjVer to Json object.
	 * @return This object converted QJsonObject.
	 */
	QJsonObject toJsonObject() const;

	/**
	 * @brief Checks if this object is less than rightSide.
	 * @param rightSide Right operand for less-than comparison.
	 * @return True if this object is less than rightSide.
	 */
	bool operator<( const ObjVer& rightSide	) const;

	/**
	 * @brief Compares this object to rightSide for equality.
	 * @param rightSide The right operand for the comparison.
	 * @return True if the compared objects are equal.
	 */
	bool operator==( const ObjVer& rightSide ) const;

// Private data.
private:

	ObjID m_id;  //!< Object id.
	int m_version;  //!< Object version.
};

/**
 * @brief Hash function impelemntation for ObjVer
 * @param key
 * @param seed Seed for the hash function.
 * @return Hash value of ObjVer.
 */
inline unsigned int qHash(const ObjVer &key, unsigned int seed)
{
	return key.type() ^ seed ^ key.id() ^ key.version();
}

}


#endif // OBJVER_H
