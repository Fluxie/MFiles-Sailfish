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

#ifndef LISTRESOURCECACHE_H
#define LISTRESOURCECACHE_H

#include <QJsonArray>
#include <QJsonValue>
#include <QMutex>
#include <QObject>
#include <QString>

#include "corebase.h"

// Forward declarations.
class QNetworkReply;
class AsyncFetch;
class VaultCore;

/**
 * @brief The ListResourceCache is an abstract base class for providing caches on top of list/array resources
 */
class ListResourceCacheBase : public CoreBase
{
	Q_OBJECT
public:

	/**
	 * @brief The Status enum represents the current operational status of the list resource cache.
	 */
	enum Status
	{
		Unpopulated,
		Refreshing,
		Ready,
	};

	/**
	 * @brief Initializes new StructureCacheBase object.
	 * @param resource The resource this cache caches. E.g /structure/classes
	 * @param parent Parent vault.
	 * @param immediateRefresh True if the cache should be refreshed immediately.
	 */
	explicit ListResourceCacheBase( const QString& resource, VaultCore* vault, VaultCore* parent = nullptr, bool immediateRefresh = true );

	/**
	 * @brief Gets all items in the cache.
	 * @return Representative for the asynchronous fetch operation.
	 */
	AsyncFetch* list() const;

	/**
	 * @brief Gets the status of the resource.
	 * @return
	 */
	Status status() const;

	/**
	 * @brief Checks if the listing is empty.
	 * @return True if this listing is currently empty.
	 */
	bool empty() const;

signals:

	/**
	 * @brief This signal is emitted when fetching items for the whole list completes.
	 * @param cookie Cookie identifies the request
	 * @param values The fetched items.
	 */
	void itemsFetched( int cookie, const QJsonArray& values ) const;

	/**
	 * @brief This signal is emitted when cache has been refreshed.
	 * @param values New values.
	 */
	void refreshed( const QJsonArray& values );

	/**
	 * @brief This signal is emitted when that status of the resource cache has changed.
	 */
	void statusChanged();

	/**
	 * @brief emptyChanged is signaled when the empty status of the model changes.
	 */
	void emptyChanged();

public slots:

	/**
	 * @brief Requestes the cache to be refreshed.
	 */
	void requestRefresh();

	/**
	 * @brief requestRefresh Request refresh of the list.
	 * @param cookie The cookie that identifies the quest.
	 */
	void requestRefreshWithCookie( int cookie );

// Protected interface.
protected:

	/**
	 * @brief Override this to clear the satellite data when the cache contents is cleared.
	 */
	virtual void clearSatelliteDataNts() {}

	/**
	 * @brief Override this to populate satellite data that after the cache contens has been refreshed.
	 */
	virtual void populateSatelliteDataNts() {}

	/**
	 * @brief Normalizes the presentation of the value. The actual normalization is implementation dependant.
	 * @param value The value that is normalized.
	 * @return Normalized value.
	 */
	virtual QJsonValue normalizeValue( QJsonValue value ) { return value; }

	/**
	 * @brief Access the actual data.
	 * @return Cached data.
	 */
	const QJsonArray& dataNts() const { return m_data; }

// Protected data.
protected:

	mutable QMutex m_mutex;  //!< Protects access to members.

// Private interface.
private:

	/**
	 * @brief getNextCookie
	 * @return Next cookie.
	 */
	int getNextCookieNts() const { return ++m_cookie; }

// Private slots.
private slots:

	/**
	 * @brief Sets the cache content from the given network reply.
	 * @param cookie Cookie identifies the request.
	 * @param reply REST API request reply.
	 */
	void setContentFrom( int cookie, QNetworkReply* reply );

// Date.
private:

	// Static data that does not need protecion.
	QString m_resource;  //!< The cached resource.

	// Dynamic variables.
	Status m_status;
	QJsonArray m_data;  //!< The actual data received via REST API.
	mutable int m_cookie;  //!< Next cookie. Cookies are used to distuinginsh requests to fething individual items from each other.
};

#endif // LISTRESOURCECACHE_H
