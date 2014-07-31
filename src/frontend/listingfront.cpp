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

#include "listingfront.h"

#include "../backend/asyncfetch.h"
#include "../backend/cachedlisting.h"

ListingFront::ListingFront( QSharedPointer< CachedListing > reference ) :
	FrontBase( reference.data() ),
	m_strongReference( reference )
{
	// Connect.
	QObject::connect( reference.data(), &CachedListing::refreshed, this, &ListingFront::refreshed );
}

//! Value list items.
AsyncFetch* ListingFront::list()
{
	// Fetch items from the base class.
	AsyncFetch* items = cachedListing()->list();
	return items;
}

QString ListingFront::resource() const
{
	return this->cachedListingConst()->resource();
}

bool ListingFront::accept( QObject* core ) const
{
	return qobject_cast< CachedListing* >( core ) != nullptr;
}

/**
 * @brief Gets the cached listing.
 * @return Reference the actual cache.
 */
CachedListing* ListingFront::cachedListing()
{
	return qobject_cast< CachedListing* >( this->core() );
}

/**
 * @brief Gets the cached listing.
 * @return Reference the actual cache.
 */
const CachedListing* ListingFront::cachedListingConst() const
{
	return qobject_cast< const CachedListing* >( this->coreConst() );
}

