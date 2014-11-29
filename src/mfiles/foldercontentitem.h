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


#ifndef FOLDERCONTENTITEM_H
#define FOLDERCONTENTITEM_H

#include <QJsonValue>
#include <QString>

#include "mfilestypewrapper.h"
#include "mfilesconstants.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of FolderContentItem M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/foldercontentitem.html">FolderContentItem</a> in M-Files REST API documentation.
 */
class FolderContentItem : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes FolderContentItem object.
	 * @param item The FolderContentItem item this object represents as JSON value.
	 */
	FolderContentItem( const QJsonValue& item );

	/**
	 * @brief Gets the type of this item.
	 * @return The type of the item.
	 */
	MFiles::Constants::FolderContentItemType type() const { return ( MFiles::Constants::FolderContentItemType ) this->property( "FolderContentItemType" ).toDouble(); }

	/**
	 * @brief Gets the object version this item represents. The type must be ObjectVersion.
	 * @return The object version.
	 */
	QJsonValue objectVersion() const { return this->property( "ObjectVersion" ); }

	/**
	 * @brief Gets the property folder as TypedValue this item represents. The type must be PropertyFolder.
	 * @return The property folder.
	 */
	QJsonValue propertyFolder() const { return this->property( "PropertyFolder" ); }

	/**
	 * @brief Gets the view that this item represents. The type must be View.
	 * @return The view.
	 */
	QJsonValue view() const { return this->property( "View" ); }

	/**
	 * @brief Gets the simple display name for this item.
	 * @return The name of this item.
	 */
	QString displayName() const;
};

}

#endif // FOLDERCONTENTITEM_H
