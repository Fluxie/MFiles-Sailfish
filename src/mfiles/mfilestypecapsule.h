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

#ifndef MFILESTYPECAPSULE_H
#define MFILESTYPECAPSULE_H

#include <QJsonObject>
#include <QJsonValue>

/**
 * @brief The MFilesTypeCapsule class
 *
 * This class encapsulates JSON object representing M-Files REST API type.
 *
 * Specific type implementations should inherit from this class.
 */
class MFilesTypeCapsule
{
public:

	/**
	 * @brief MFilesTypeCapsule
	 * @param value The value this capsule represents.
	 */
	MFilesTypeCapsule( const QJsonValue& value );

	/**
	 * @brief MFilesTypeCapsule
	 */
	MFilesTypeCapsule() {}

	/**
	 * @brief isEmpty
	 * @return True if this value empty or undefined.
	 */
	bool isUndefined() const { return m_object.isEmpty(); }

	//! indexing operator.
	QJsonValueRef operator[]( const QString& key ) { return m_object[ key ]; }

	/**
	 * @brief value
	 * @return The object as JSON value.
	 */
	QJsonValue value() const { return QJsonValue( m_object ); }

// Protected interface.
protected:

	//! Accesses the object.
	const QJsonObject& object() const { return m_object; }

	//! Accesses the object.
	QJsonObject& object() { return m_object; }

	/**
	 * @brief contains
	 * @param field The name of the field in encapsuled Json object.
	 * @return True if the filed is included in the Json object.
	 */
	bool contains( const QString& field ) const { return m_object.contains( field ); }

// Private data.
private:

	QJsonObject m_object;
};

#endif // MFILESTYPECAPSULE_H
