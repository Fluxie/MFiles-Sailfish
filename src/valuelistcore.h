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

#ifndef VALUELISTCORE_H
#define VALUELISTCORE_H

#include <QHash>
#include <QJsonValue>
#include <QObject>
#include <QString>
#include <QVector>

#include "structurecachebase.h"
#include "typedvaluefilter.h"

// Forward declarations.
class VaultCore;

class ValueListCore : public StructureCacheBase
{
	Q_OBJECT
public:

	//! Constructs value list object to represent one value list.
	explicit ValueListCore( VaultCore* vault, int valueList, int owner );

	//! Constructs value list object to represent one value list accessed via the specified property definition.
	explicit ValueListCore( VaultCore* vault, int valueList, int owner, const TypedValueFilter* filter );

	//! Destructor.
	virtual ~ValueListCore();

	//! The id of the value list.
	int id() const { return m_valueList; }

	/**
	 * @brief availableItems fetches all the specified items that are currently available.
	 * @param ids A collection of ids of items that are fetched.
	 * @return All specified value list items that are available.
	 */
	AsyncFetch* availableItems( const QSet< int > ids );

	/**
	 * @brief owner value list
	 * @return Id of the owner value list.
	 */
	int owner() const { return m_owner; }

	//! Filter the values in this list are filtered with.
	const TypedValueFilter* filter() const { return m_filter; }

signals:

public slots:

// Private interface.
private:

	//! Gets resource for fetching the relevant value list items.
	static QString getResource( bool allItems, int valueList, int owner, const TypedValueFilter* filter );

// Private data.
private:

	// Static data that does not need protection.
	int m_valueList;  //!< The id of the value list this object represents.
	int m_owner;  //!< The id of the owner of this value list.
	TypedValueFilter* m_filter;  //!< Filter for retrieving the values from the server.

};

#endif // VALUELISTCORE_H
