#ifndef LISTINGFRONT_H
#define LISTINGFRONT_H

#include <QSharedPointer>

#include "frontbase.h"

// Forward declarations.
class AsyncFetch;
class CachedListing;

/**
 * @brief The ListingFront class represents a cached listing in UI.
 */
class ListingFront : public FrontBase
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new listing front.
	 * @param reference The referred listing.
	 */
	explicit ListingFront( QSharedPointer< CachedListing > reference );

	/**
	 * @brief Initializes new listing front.
	 */
	ListingFront();

	//! Value list items.
	Q_INVOKABLE virtual AsyncFetch* list();

	/**
	 * @brief Gets the resource for fetching this listing.
	 * @return The resource for fetching this listing.
	 */
	QString resource() const;

signals:

	/**
	 * @brief refreshed is signaled when the listing content changes.
	 */
	void refreshed();

public slots:

// FrontBase.
protected:

	//! Does this front accept the specified core.
	virtual bool accept( QObject* core ) const;

private:

	/**
	 * @brief Gets the cached listing.
	 * @return Reference the actual cache.
	 */
	CachedListing* cachedListing();

	/**
	 * @brief Gets the cached listing.
	 * @return Reference the actual cache.
	 */
	const CachedListing* cachedListingConst() const;

private:

	QSharedPointer< CachedListing > m_strongReference;

};

#endif // LISTINGFRONT_H
