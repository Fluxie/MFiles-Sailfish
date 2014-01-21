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

#include "mfilestypecapsule.h"

class PropertyDef : public MFilesTypeCapsule
{
public:

	/**
	 * @brief PropertyDef
	 */
	PropertyDef( const QJsonValue& propertyDef );

	/**
	 * @brief id
	 * @return Id of the property definition.
	 */
	int id() const { return this->object()[ "ID" ].toDouble(); }

	/**
	 * @brief dataType
	 * @return Data type of the property definition.
	 */
	int dataType() const { return this->object()[ "DataType" ].toDouble(); }

	/**
	 * @brief basedOnValueList
	 * @return True if the property definition is based on a value list.
	 */
	bool basedOnValueList() const { return this->object()[ "BasedOnValueList" ].toBool(); }

	/**
	 * @brief valueList
	 * @return The id of the value list this property definition represents.
	 */
	int valueList() const { Q_ASSERT( this->object().contains( "ValueList" ) );  return this->object()[ "ValueList" ].toDouble(); }

	QJsonValue ownerPropertyDef() const { Q_ASSERT( this->object().contains( "OwnerPropertyDef" ) );  return this->object()[ "OwnerPropertyDef" ]; }

	/**
	 * @brief isValidOwnerFor checks wheather this is a valid owner property definition for subItemRepresentative or not.
	 * @param subItemRepresentative
	 * @return True if this property definition is a valid owner property for the given sub item.
	 */
	bool isValidOwnerFor( const PropertyDef& subItemRepresentative ) const;

};

#endif // PROPERTYDEF_H
