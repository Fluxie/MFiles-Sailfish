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

#include "cachedlisting.h"

#include <QJsonObject>
#include <QMutexLocker>
#include <QNetworkReply>

#include "asyncfetch.h"
#include "listingcache.h"
#include "../mfiles/foldercontentitem.h"
#include "../mfiles/typedvalue.h"
#include "../mfiles/view.h"
#include "../mfiles/viewlocation.h"

CachedListing::CachedListing( ListingCache* parent, const ListingId& id ) :
	ListResourceCacheBase( id.path(), parent->vault(), nullptr, false ),
	m_id( id ),
	m_owner( parent )
{

	qDebug( "Cached listing instantiated." );
}

QString CachedListing::resource() const
{
	return m_id.path();
}

QJsonValue CachedListing::normalizeValue( QJsonValue value )
{
	MFiles::FolderContentItem item( value );
	if( item.type() == MFiles::Constants::ViewFolder )
	{
		// Normalize the view location of the view.
		QJsonValue viewValue = value.toObject()[ "View" ];
		if( ! viewValue.toObject().contains( "ViewLocation" ) )
		{
			MFiles::ViewLocation emptyLocation;
			QJsonObject viewObject = viewValue.toObject();
			viewObject[ "ViewLocation" ] = emptyLocation.toJsonValue();
			viewValue = viewObject;
		}
		else
		{
			// Normalize the overlapping property of the view location.
			QJsonObject viewLocationObject = viewValue.toObject()[ "ViewLocation" ].toObject();
			if( ! viewLocationObject.contains( "Overlapping" ) )
			{
				viewLocationObject[ "Overlapping" ] = QJsonValue( false );
				QJsonObject viewObject = viewValue.toObject();
				viewObject[ "ViewLocation" ] = viewLocationObject;
				viewValue = viewObject;
			}

		}  // end if

		// Normalize the name of the view.
		MFiles::View view( viewValue );
		MFiles::ViewLocation location( view.viewLocation() );
		if( location.isOverlapping()  )
		{
			// The folder is overlapping.
			// Use the name of the overlapping folder as the name of the view.
			MFiles::TypedValue folder( location.overlappderFolder() );
			QJsonObject viewObject = viewValue.toObject();
			viewObject[ "Name" ] = folder.displayValue();
			viewValue = viewObject;
		}

		// Normalize the view proeprty of the folder content item.
		QJsonObject itemObject = value.toObject();
		itemObject[ "View" ] = viewValue;
		value = itemObject;

	}  // end if

	return value;
}
