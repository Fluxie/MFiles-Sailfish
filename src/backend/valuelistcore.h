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

/**
 * @brief The ValueListCore class stores value list items of the class fetched from the server.
 */
class ValueListCore : public StructureCacheBase
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new ValueListCore object to represent one value list.
	 * @param vault Vault.
	 * @param valueList  Id of the value list.
	 * @param owner Id of the owner value list.
	 */
	explicit ValueListCore( VaultCore* vault, int valueList, int owner );

	/**
	 * @brief Initializes new ValueListCore object to represent one value list with a filter.
	 * @param vault Vault.
	 * @param valueList  Id of the value list.
	 * @param owner Id of the owner value list.
	 * @param filter Filter for filtering the value list items.
	 */
	explicit ValueListCore( VaultCore* vault, int valueList, int owner, const TypedValueFilter* filter );

	//! Destructor.
	virtual ~ValueListCore();

	/**
	 * @brief Gets the id of the value list.
	 * @return Id of the value list.
	 */
	int id() const { return m_valueList; }

	/**
	 * @brief Gets the owner of the value list.
	 * @return Id of the owner value list.
	 */	
	int owner() const { return m_owner; }

	/**
	 * @brief Gets the filter that is used to filter the items in this value list.
	 * @return Value list's filter.
	 */
	const TypedValueFilter* filter() const { return m_filter; }

signals:

public slots:

protected:

	/**
	 * @brief Normalizes the presentation of the value.
	 * @param value The value that is normalized.
	 * @return Normalized value.
	 */
	virtual QJsonValue normalizeValue( QJsonValue value );

	/**
	 * @brief Initializes new ValueListCore object to represent one value list with a filter.
	 * @param resource Resouce for fetching all items.
	 * @param fetchOne Resource for fetching one item.
	 * @param vault Vault.
	 * @param valueList  Id of the value list.
	 * @param owner Id of the owner value list.
	 * @param filter Filter for filtering the value list items.
	 */
	explicit ValueListCore(
			const QString& resource, const QString& fetchOne,
			VaultCore* vault, int valueList, int owner, const TypedValueFilter* filter );

// Private interface.
private:

	/**
	 * @brief Gets resource for fetching the relevant value list items.
	 * @param allItems True to get resource for fetching all items. False to get resource for fetching one item.
	 * @param valueList Id of the value list.
	 * @param owner Possible owner of the value list. Maybe be required with certain filters.
	 * @param filter Filter to apply to the resource.
	 * @return Returns MFWS REST API resource for fetching the specified values.
	 */
	static QString getResource( bool allItems, int valueList, int owner, const TypedValueFilter* filter );

// Private data.
private:

	// Static data that does not need protection.
	int m_valueList;  //!< The id of the value list this object represents.
	int m_owner;  //!< The id of the owner of this value list.
	TypedValueFilter* m_filter;  //!< Filter for retrieving the values from the server.

};

#endif // VALUELISTCORE_H
