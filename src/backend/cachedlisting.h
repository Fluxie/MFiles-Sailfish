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

// Private data.
private:

	ListingId m_id;
	ListingCache* m_owner;
};

#endif // CACHEDLISTING_H
