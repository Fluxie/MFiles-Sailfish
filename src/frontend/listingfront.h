#ifndef LISTINGFRONT_H
#define LISTINGFRONT_H

#include <QJsonArray>
#include <QSharedPointer>

#include "frontbase.h"

#include "listingstatus.h"

// Forward declarations.
class AsyncFetch;
class CachedListing;

/**
 * @brief The ListingFront class represents a cached listing in UI.
 */
class ListingFront : public FrontBase
{
	Q_OBJECT
	Q_PROPERTY( ListingStatus::Status status READ status NOTIFY statusChanged )
	Q_PROPERTY( bool empty READ empty NOTIFY emptyChanged )
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

	/**
	 * @brief Gets the status of the listing.
	 * @return Status of the listing.
	 */
	ListingStatus::Status status() const;

	/**
	 * @brief Checks if the listing is empty.
	 * @return True if this listing is currently empty.
	 */
	bool empty() const;

signals:

	/**
	 * @brief refreshed is signaled when the listing content changes.
	 * @param values New values.
	 */
	void refreshed( const QJsonArray& values );

	/**
	 * @brief statusChanged is signaled when the status of the listing changes.
	 */
	void statusChanged();

	/**
	 * @brief emptyChanged is signaled when the empty status of the model changes.
	 */
	void emptyChanged();

public slots:

	/**
	 * @brief Requests for an asynchronous refresh.
	 */
	void requestRefresh();

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
