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

#ifndef VIEWLOCATION_H
#define VIEWLOCATION_H

#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of ViewLocation M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/viewlocation.html">ViewLocation</a> in M-Files REST API documentation.
 */
class ViewLocation : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes view location based on existing Json object.
	 * @param location The wrapped Json object.
	 */
	ViewLocation( const QJsonValue& location );

	/**
	 * @brief Initializes new empty view location.
	 */
	ViewLocation();

	/**
	 * @brief Gets the overlapped folder description. Valid only if isOverlapping is true.
	 * @return The overlapped folder description.
	 */
	QJsonValue overlappderFolder() const { return this->property( "OverlappedFolder" ); }

	/**
	 * @brief Checks if the view is an overlapping view.
	 * @return True if the view is an overlapping view.
	 */
	bool isOverlapping() const { return this->property( "Overlapping" ).toBool(); }

};

}

#endif // VIEWLOCATION_H
