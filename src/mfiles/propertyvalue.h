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

#include "mfilestypecapsule.h"
#include "typedvalue.h"

/**
 * @brief The PropertyValue class
 */
class PropertyValue : public MFilesTypeCapsule
{
public:

	/**
	 * @brief Initializes new property value.
	 */
	PropertyValue( const QJsonValue& propertyValue );

	/**
	 * @brief Initializes new property value.
	 */
	PropertyValue( int propertyDefId, const TypedValue& typedValue );	

	/**
	 * @brief hasValue
	 * @return  True if this
	 */
	bool hasValue() const { return ! this->isUndefined() && this->typedValue().hasValue(); }

	//! The id of the property definition.
	int propertyDef() const { return this->object()[ "PropertyDef" ].toDouble(); }

	//! Typed value.
	TypedValue typedValue() const { return TypedValue( this->object()[ "TypedValue" ] ); }

};

#endif // PROPERTYVALUE_H
