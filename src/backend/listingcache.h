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
