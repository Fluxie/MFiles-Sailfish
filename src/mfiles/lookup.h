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
 * @brief The Lookup class
 */
class Lookup : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes new Lookup.
	 */
	Lookup( const QJsonValue& lookup );

	//! The id of the property definition.
	int item() const { return this->object()[ "Item" ].toDouble(); }

	/**
	 * @brief displayValue
	 * @return The display value of the lookup.
	 */
	QString displayValue() const { Q_ASSERT( this->object().contains( "DisplayValue" ) ); return this->object()[ "DisplayValue" ].toString(); }

};

}

inline uint qHash( const MFiles::Lookup& lookup )
{
	uint hash = lookup.item();
	return hash;
}

#endif // LOOKUP_H
