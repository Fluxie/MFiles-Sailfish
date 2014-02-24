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
class ValueListItem
{
public:

	/**
	 * @brief Initializes value list item based on existing Json object.
	 * @param typedValue The wrapped Json object.
	 */
	ValueListItem( const QJsonValue& valueListItem );

	/**
	 * @brief Initializes empty ValueListItem.
	 */
	ValueListItem() {}

	/**
	 * @brief Checks if the value list item is empty?
	 * @return True if the value list item is empty / unspecified.
	 */
	bool isEmpty() const { return m_valueListItem.isEmpty(); }

	/**
	 * @brief Gets the id of the value list item.
	 * @return Value list item id.
	 */
	int id() const { return m_valueListItem[ "ID" ].toDouble(); }

	/**
	 * @brief name
	 * @return The name of the value list item.
	 */
	QString name() const { return m_valueListItem[ "Name" ].toString(); }

	/**
	 * @brief Checks if this value list item has an owner item.
	 * @return True if this value list item has an owner.
	 */
	bool hasOwner() const { return m_valueListItem[ "HasOwner" ].toBool(); }

	/**
	 * @brief Gets the id of the possible owner.
	 * @return The id of the owner item.
	 */
	int ownerId() const { return m_valueListItem[ "OwnerID" ].toDouble(); }

	/**
	 * @brief Converts this value list item to lookup.
	 * @return This value list item as a lookup.
	 */
	QJsonValue toLookup() const;

// Private data.
private:

	QJsonObject m_valueListItem;
};

}

#endif // VALUELISTITEM_H
