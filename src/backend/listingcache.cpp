#include "listingcache.h"

#include <QMutexLocker>

#include "vaultcore.h"


ListingCache::ListingCache( VaultCore* parent ) :
	CoreBase( parent, parent )
{
	m_root = QSharedPointer< CachedListing >::create( this, ListingId::Root );

	// Connect errors.
	QObject::connect( m_root.data(), &CoreBase::error, this, &CoreBase::reportError );
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
		if( listing->isNull() )
		{
			listing = new QSharedPointer< CachedListing >( new CachedListing( this, id ) );
			QObject::connect( listing->data(), &CoreBase::error, this, &CoreBase::reportError );
			m_cache.insert( id, listing );
		}
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
