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

/**
 * @brief C++ representation of ObjID object.
 */
class ObjID
{
public:

	/**
	 * @brief Initializes new ObjID object.
	 * @param type The type of the object.
	 * @param id The id of the object.
	 */
	ObjID( int type, int id );

	/**
	 * @brief Initializes new ObjID object.
	 * @param json Json object that identifies the object.
	 */
	ObjID( const QJsonObject& json );

	/**
	 * @brief Gets the object type.
	 * @return The object type.
	 */
	int type() const { return m_type; }

	/**
	 * @brief Gets the object id.
	 * @return The object id.
	 */
	int id() const { return m_id; }

	/**
	 * @brief Converts this ObjID to QJsonObject.
	 * @return This ObjID as QJsonObject.
	 */
	QJsonObject toJsonObject() const;

	/**
	 * @brief operator <
	 * @param rightSide Right operan of the less operator.
	 * @return True this object is less than rightSide.
	 */
	bool operator< ( const ObjID& rightSide ) const;

	/**
	 * @brief Compares objects for equality.
	 * @param rightSide The object this object is compared with.
	 * @return True if the compared objects are equal.
	 */
	bool operator== ( const ObjID& rightSide	) const;

	/**
	 * @brief Compares objects for equality.
	 * @param rightSide The object this object is compared with.
	 * @return True if the compared objects are not equal.
	 */
	bool operator!=( const ObjID& rightSide ) const	{ return ! ( (*this) == rightSide ); }

// Private data.
private:

	int m_type;  //!< Object type.
	int m_id;  //!< Object id.
};

/**
 * @brief Hash function for ObjID object.
 * @param key The ObjID object for which the hash value is calculated.
 * @param seed Seed for the hash function.
 * @return Hash value of the ObjID object.
 */
inline unsigned int qHash( const ObjID &key, unsigned int seed )
{
	return seed ^ key.type() ^ key.id();
}

/**
 * @brief Hash function for ObjID object.
 * @param key The ObjID object for which the hash value is calculated.
 * @return Hash value of the ObjID object.
 */
inline unsigned int qHash( const ObjID &key )
{
	return key.type() ^ key.id();
}

}


#endif // OBJID_H
