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

#ifndef VIEW_H
#define VIEW_H

#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of View M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/view.html">View</a> in M-Files REST API documentation.
 */
class View : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes view based on existing Json object.
	 * @param view The wrapped Json object.
	 */
	View( const QJsonValue& view );

	/**
	 * @brief Gets the id of the view.
	 * @return The id of the view.
	 */
	int id() const { return this->property( "ID" ).toDouble(); }

	/**
	 * @brief Gets the common view status of this view.
	 * @return True if this view is a common view.
	 */
	bool common() const { return this->property( "Common" ).toBool(); }

	/**
	 * @brief Gets information about the view location.
	 * @return The view location.
	 */
	QJsonValue viewLocation() const { return this->property( "ViewLocation" ); }

	/**
	 * @brief isBuiltIn
	 * @return True if this view is a built-in view.
	 */
	bool builtIn() const { return this->id() < 100; }
};

}

#endif // VIEW_H
