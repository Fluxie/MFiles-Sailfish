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

#ifndef OBJECTVERSION_H
#define OBJECTVERSION_H

#include <QJsonValue>

#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of ObjectVersion M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/objectversion.html">ObjectVersion</a> in M-Files REST API documentation.
 */
class ObjectVersion : MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes ObjectVersion object.
	 * @param item The ObjectVersion this object represents as JSON value.
	 */
	ObjectVersion( const QJsonValue& objectVersion );

	/**
	 * @brief Gets the identity of this object as ObjVer.
	 * @return The ObjVer.
	 */
	QJsonValue objVer() const { return this->property( "ObjVer" ); }

	/**
	 * @brief Gets the title of this object version.
	 * @return The title of this object
	 */
	QString title() const { return this->property( "Title" ).toString(); }
};

}

#endif // OBJECTVERSION_H
