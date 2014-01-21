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

class ValueListItem
{
public:

	/**
	 * @brief Initializes new value list item.
	 */
	ValueListItem( const QJsonValue& valueListItem );

	/**
	 * @brief Initializes empty ValueListItem.
	 */
	ValueListItem() {}

	/**
	 * @brief isEmpty
	 * @return True if the value list item is empty / unspecified.
	 */
	bool isEmpty() const { return m_valueListItem.isEmpty(); }

	/**
	 * @brief id
	 * @return  Valuelist item id.
	 */
	int id() const { return m_valueListItem[ "ID" ].toDouble(); }

	/**
	 * @brief name
	 * @return The name of the value list item.
	 */
	QString name() const { return m_valueListItem[ "Name" ].toString(); }

	/**
	 * @brief hasOwner
	 * @return True if this value list item has an owner.
	 */
	bool hasOwner() const { return m_valueListItem[ "HasOwner" ].toBool(); }

	/**
	 * @brief hasOwner
	 * @return The id of the owner item.
	 */
	int ownerId() const { return m_valueListItem[ "OwnerID" ].toDouble(); }

	//! Converts this value list item to lookup.
	QJsonValue toLookup() const;

// Private data.
private:

	QJsonObject m_valueListItem;
};

#endif // VALUELISTITEM_H
