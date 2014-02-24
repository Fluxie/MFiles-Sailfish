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

#ifndef OWNERPROPERTY_H
#define OWNERPROPERTY_H

#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of OwnerProperty M-Files REST API Json object.
 */
class OwnerProperty : public MFilesTypeWrapper
{
public:

	/**
	 * @brief OwnerProperty
	 */
	OwnerProperty( const QJsonValue& ownerProperty );

	/**
	 * @brief Gets the id of the owner property definition.
	 * @return The id of the owner property definition.
	 */
	int id() const { Q_ASSERT( this->contains( "ID" ) ); return this->object()[ "ID" ].toDouble(); }

	/**
	 * @brief isRelationFiltering
	 * @return True if relation filtering is in use.
	 */
	bool isRelationFiltering() const { Q_ASSERT( this->contains( "IsRelationFiltering" ) ); return this->object()[ "IsRelationFiltering" ].toBool(); }
};

}

#endif // OWNERPROPERTY_H
