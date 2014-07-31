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

#include "foldercontentitem.h"

MFiles::FolderContentItem::FolderContentItem( const QJsonValue& item ) :
	MFilesTypeWrapper( item )
{
}

QString MFiles::FolderContentItem::dipslayName() const
{
	// Return the default display name based on the type of the item.
	switch( this->type() )
	{
	case MFiles::Constants::ObjectVersion :
		return this->objectVersion().toObject()[ "Title" ].toString();
		break;

	case MFiles::Constants::PropertyFolder :
		return this->propertyFolder().toObject()[ "DisplayValue" ].toString();
		break;

	case MFiles::Constants::ViewFolder :
		return this->view().toObject()[ "Name" ].toString();
		break;

	default :
		Q_ASSERT( false );
		return "";
	}
}
