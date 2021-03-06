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

#ifndef MFILESTYPEWRAPPER_H
#define MFILESTYPEWRAPPER_H

#include <QJsonObject>
#include <QJsonValue>

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Base class for M-Files type wrappers for QJsonValue objects.
 *
 * @remarks {
 * This class encapsulates JSON object representing M-Files REST API type.
 *
 * Specific type implementations should inherit from this class. }
 *
 * @see Different <a href="http://www.m-files.com/mfws/structs.html">Json object types</a> available in M-Files REST API.
 */
class MFilesTypeWrapper
{
public:

	/**
	 * @brief MFilesTypeWrapper
	 * @param value The value this capsule represents.
	 */
	MFilesTypeWrapper( const char* filename, const QJsonValue& value );

	/**
	 * @brief MFilesTypeWrapper
	 */
	MFilesTypeWrapper() {}

	/**
	 * @brief isEmpty
	 * @return True if this value empty or undefined.
	 */
	bool isUndefined() const { return m_object.isEmpty(); }

	//! indexing operator.
	QJsonValueRef operator[]( const QString& key ) { return m_object[ key ]; }

	//! indexing operator.
	const QJsonValue operator[]( const QString& key ) const { return m_object[ key ]; }

	/**
	 * @brief value
	 * @return The object as JSON value.
	 */
	QJsonValue toJsonValue() const { return QJsonValue( m_object ); }

	/**
	 * @brief Check if value can be a valid M-Files type.
	 * @param value The value to check.
	 * @return True if the value is null or epmty.
	 */
	static bool isValid( const QJsonValue& value );

// Protected interface.
protected:

	/**
	 * @brief Returns constant reference to the wrapped object.
	 * @return Constant reference to the wrapped object.
	 */
	const QJsonObject& object() const { return m_object; }

	/**
	 * @brief Returns reference to the wrapped object.
	 * @return Constant reference to the wrapped object.
	 */
	QJsonObject& object() { return m_object; }

	/**
	 * @brief Checks if the specified field is set in the wrapped QJsonValue object.
	 * @param field The name of the field in encapsuled Json object.
	 * @return True if the filed is included in the Json object.
	 */
	bool contains( const QString& field ) const { return m_object.contains( field ); }

	/**
	 * @brief Accesses the value of the specified property.
	 * @param field The name of the property.
	 * @return The Json value of the propetty.
	 */
	const QJsonValue property( const QString& field ) const { Q_ASSERT_X( this->contains( field ), field.toLatin1(), "Field missing." ); return m_object[ field ]; }

// Private data.
private:

	QJsonObject m_object;
};

}

#endif // MFILESTYPEWRAPPER_H
