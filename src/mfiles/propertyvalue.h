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

#ifndef PROPERTYVALUE_H
#define PROPERTYVALUE_H

#include <QJsonValue>
#include <QJsonObject>

#include "mfilestypewrapper.h"
#include "typedvalue.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of PropertyValue M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/propertyvalue.html">PropertyValue</a> in M-Files REST API documentation.
 */
class PropertyValue : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes property value object based on existing Json object.
	 * @param propertyValue The wrapped Json object.
	 */
	PropertyValue( const QJsonValue& propertyValue );

	/**
	 * @brief Initializes new property value object.
	 * @param propertyDefId Property definition id this property value represents.
	 * @param typedValue The value part of this property value.
	 */
	PropertyValue( int propertyDefId, const TypedValue& typedValue );	

	/**
	 * @brief Checks if this property value has a valid value.
	 * @return  True if a value has been set.
	 */
	bool hasValue() const { return ! this->isUndefined() && this->typedValue().hasValue(); }

	/**
	 * @brief Gets the id of the property definition.
	 * @return The id of the proeprty definition.
	 */
	int propertyDef() const { return this->object()[ "PropertyDef" ].toDouble(); }

	/**
	 * @brief Gets the typed value.
	 * @return The typed value of this property value.
	 */
	TypedValue typedValue() const { return TypedValue( this->object()[ "TypedValue" ] ); }

};

}

#endif // PROPERTYVALUE_H
