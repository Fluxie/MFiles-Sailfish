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

#ifndef CACHEDLISTING_H
#define CACHEDLISTING_H

#include <QJsonArray>
#include <QMutex>
#include <QObject>

#include "listingid.h"
#include "listresourcecachebase.h"

// Forward declarations.
class AsyncFetch;
class ListingCache;

/**
 * @brief The CachedListing class
 */
class CachedListing : public ListResourceCacheBase
{
    Q_OBJECT
public:

	/**
	 * @brief Initializes CachedListing object
	 * @param id The id of this listing.
	 * @param parent Listing cache that owns this object.
	 */
	explicit CachedListing( ListingCache* parent, const ListingId& id );

	/**
	 * @brief Gets the resource for fetching this listing.
	 * @return The resource for fetching this listing.
	 */
	QString resource() const;

// ListResourceCacheBase.
protected:

	virtual QJsonValue normalizeValue( QJsonValue value );

// Private data.
private:

	ListingId m_id;
	ListingCache* m_owner;
};

#endif // CACHEDLISTING_H
