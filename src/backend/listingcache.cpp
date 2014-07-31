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

#include "listingcache.h"

#include <QMutexLocker>

#include "vaultcore.h"


ListingCache::ListingCache( VaultCore* parent ) :
	CoreBase( parent, parent )
{
	Q_CHECK_PTR( parent );

	m_root = QSharedPointer< CachedListing >::create( this, ListingId::Root );

	// Connect errors.
	QObject::connect( m_root.data(), &CoreBase::error, this, &CoreBase::reportError );

	qDebug( "ListingCache initialized." );
}

/**
 * @brief root
 * @return The root listing.
 */
QSharedPointer< CachedListing > ListingCache::root()
{
	// Return the root. It is always available.
	return m_root;
}

/**
 * @brief Gets the specified listing.
 * @param id Idenfities the listing
 * @return REturns the requested listin.
 */
QSharedPointer< CachedListing > ListingCache::listing( const ListingId& id )
{
	QSharedPointer< CachedListing >* listing = nullptr;
	{
		QMutexLocker lock( &m_mutex );

		// Get the item from the listing or create a new one if it does not exist.
		listing = m_cache.object( id );
		if( listing == nullptr )
		{
			listing = new QSharedPointer< CachedListing >( new CachedListing( this, id ) );
			QObject::connect( listing->data(), &CoreBase::error, this, &CoreBase::reportError );
			m_cache.insert( id, listing );
		}
		Q_ASSERT( ! listing->isNull() );
	}
	return *listing;
}

/**
 * @brief Requestes the cache to be refreshed.
 */
void ListingCache::requestRefresh()
{
	// Clear the cache.
	{
		QMutexLocker lock( &m_mutex );

		m_cache.clear();
	}

	// Refresh the root listing.
	m_root->requestRefresh();
}
