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

#ifndef LISTINGCACHE_H
#define LISTINGCACHE_H

#include <QCache>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>


#include "cachedlisting.h"
#include "corebase.h"
#include "listingid.h"

// Forward declarations.
class VaultCore;

/**
 * @brief The ListingCache class
 */
class ListingCache : public CoreBase
{
    Q_OBJECT
public:

	/**
	 * @brief ListingCache
	 * @param parent Parent vault
	 */
	explicit ListingCache( VaultCore* parent );

	/**
	 * @brief root
	 * @return The root listing.
	 */
	QSharedPointer< CachedListing > root();

	/**
	 * @brief Gets the specified listing.
	 * @param id Idenfities the listing
	 * @return REturns the requested listin.
	 */
	QSharedPointer< CachedListing > listing( const ListingId& id );

signals:

	/**
	 * @brief Signaled when the root listing has changed.
	 */
	void rootChanged();

public slots:

	/**
	 * @brief Requestes the cache to be refreshed.
	 */
	void requestRefresh();

// Private data.
private:

	friend class CachedListing;

	mutable QMutex m_mutex;
	QSharedPointer< CachedListing > m_root;
	QCache< ListingId, QSharedPointer< CachedListing > > m_cache;  //!< Cache of listings.
};

#endif // LISTINGCACHE_H
