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

#ifndef LOOKUP_H
#define LOOKUP_H

#include <QJsonObject>
#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of Lookup M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/lookup.html">Lookup</a> in M-Files REST API documentation.
 */
class Lookup : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes lookup based on existing Json object.
	 * @param lookup The wrapped lookup.
	 */
	Lookup( const QJsonValue& lookup );

	/**
	 * @brief Gets the id of the lookup.
	 * @return The id of the lookup.
	 */
	int item() const { return this->property( "Item" ).toDouble(); }

	/**
	 * @brief Gets the display value of this lookup.
	 * @return The display value of the lookup.
	 */
	QString displayValue() const { return this->property( "DisplayValue" ).toString(); }

};

}

/**
 * @brief Hash function for Lookup object.
 * @param lookup The Lookup object for which the hash value is calculated.
 * @return Hash value of the ObjID object.
 */
inline uint qHash( const MFiles::Lookup& lookup )
{
	uint hash = lookup.item();
	return hash;
}

#endif // LOOKUP_H
