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

#ifndef PROPERTYDEF_H
#define PROPERTYDEF_H

#include <QJsonObject>
#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of PropertyDef M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/propertydef.html">PropertyDef</a> in M-Files REST API documentation.
 */
class PropertyDef : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes PropertyDef wrapper object.
	 * @param properytDef M-Files REST API Json object.
	 */
	PropertyDef( const QJsonValue& propertyDef );

	/**
	 * @brief Gets the property definition id.
	 * @return Id of the property definition.
	 */
	int id() const { return this->object()[ "ID" ].toDouble(); }

	/**
	 * @brief Gets the data type of the property definition.
	 * @return Data type of the property definition.
	 */
	int dataType() const { return this->object()[ "DataType" ].toDouble(); }

	/**
	 * @brief Checks is this PropertyDef based on a value list.
	 * @return True if the property definition is based on a value list.
	 */
	bool basedOnValueList() const { return this->object()[ "BasedOnValueList" ].toBool(); }

	/**
	 * @brief Gets the value list this property definition represents.
	 * @return The id of the value list this property definition represents.
	 */
	int valueList() const { Q_ASSERT( this->object().contains( "ValueList" ) );  return this->object()[ "ValueList" ].toDouble(); }

	/**
	 * @brief Gets the owner property definition.
	 * @return Json value representing the owner property definition.
	 */
	QJsonValue ownerPropertyDef() const { Q_ASSERT( this->object().contains( "OwnerPropertyDef" ) );  return this->object()[ "OwnerPropertyDef" ]; }

	/**
	 * @brief Checks wheather this is a valid owner property definition for subItemRepresentative or not.
	 * @param subItemRepresentative Property definition candidate for represing sub-items of this property definition.
	 * @return True if this property definition is a valid owner property for the given sub item.
	 */
	bool isValidOwnerFor( const PropertyDef& subItemRepresentative ) const;

};

}

#endif // PROPERTYDEF_H
