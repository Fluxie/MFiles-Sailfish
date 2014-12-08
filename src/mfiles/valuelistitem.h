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

#ifndef VALUELISTITEM_H
#define VALUELISTITEM_H

#include <QJsonObject>
#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of ValueListItem M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/valuelistitem.html">ValueListItem</a> in M-Files REST API documentation.
 */
class ValueListItem : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes value list item based on existing Json object.
	 * @param valueListItem The wrapped Json object.
	 */
	ValueListItem( const QJsonValue& valueListItem );

	/**
	 * @brief Initializes empty ValueListItem.
	 * @param valueListId The id of the value list.
	 * @param id The id of this item..
	 * @param name The name of this item.
	 */
	ValueListItem( int valueListId, int id, const QString& name );

	/**
	 * @brief Checks if the value list item is empty?
	 * @return True if the value list item is empty / unspecified.
	 */
	bool isEmpty() const { return this->object().isEmpty(); }

	/**
	 * @brief Gets the id of the value list item.
	 * @return Value list item id.
	 */
	int id() const { return this->property( "ID" ).toDouble(); }

	/**
	 * @brief name
	 * @return The name of the value list item.
	 */
	QString name() const { return this->property( "Name" ).toString(); }

	/**
	 * @brief Checks if this value list item has an owner item.
	 * @return True if this value list item has an owner.
	 */
	bool hasOwner() const { return this->property( "HasOwner" ).toBool(); }

	/**
	 * @brief Checks if this value list item can be selected for an object.
	 * @return True if the user can select this value list item.
	 */
	bool canSelect() const { return this->property( "CanSelect" ).toBool(); }

	/**
	 * @brief Sets the CanSelect flag for this item.
	 * @param canSelect True if this item can be selected.
	 */
	void setCanSelect( bool canSelect ) { this->object()[ "CanSelect" ] = canSelect; }

	/**
	 * @brief Gets the id of the possible owner.
	 * @return The id of the owner item.
	 */
	int ownerId() const { return this->property( "OwnerID" ).toDouble(); }

	/**
	 * @brief Converts this value list item to lookup.
	 * @return This value list item as a lookup.
	 */
	QJsonValue toLookup() const;

// Private data.
private:

};

}

#endif // VALUELISTITEM_H
