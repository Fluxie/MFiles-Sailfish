/*
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

#ifndef STRUCTURECACHEBASE_H
#define STRUCTURECACHEBASE_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>
#include <QHash>

#include "corebase.h"

// Foward declarations.
class AsyncFetch;
class VaultCore;
class MfwsRest;
class QNetworkReply;

class StructureCacheBase : public CoreBase
{

	Q_OBJECT
public:

	//! Constructor.
	explicit StructureCacheBase(
		const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
		VaultCore* parent,  //!< Parent vault.
		bool immediateRefresh = true  //!< True if the cache should be refreshed immediately.
	);

	//! Constructor.
	explicit StructureCacheBase(
		const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
		const QString& fetchOne,  //!< String format for fetching one item.
		VaultCore* parent,  //!< Parent vault.
		bool immediateRefresh = true  //!< True if the cache should be refreshed immediately.
	);

	//! Destructor.
	virtual ~StructureCacheBase() {}

	//! Gets an item from the cache.
	AsyncFetch* get( int id ) const;

	/**
	 * @brief list
	 * @return The values of the value list.
	 */
	AsyncFetch* list() const;

	//! Is this cache populated?
	bool populated() const { return m_populated; }
	
signals:

	//! Signaled when cache is unable to server a request from the cache.
	void missing( int id ) const;

	//! Signaled when fething of an item completes
	void itemFetched( int cookie, const QJsonValue& value ) const;

	/**
	 * @brief itemsFetched is signaled when fetching items for the whole list completes.
	 * @param cookie Cookie identifies the request
	 * @param values The fetched items.
	 */
	void itemsFetched( int cookie, const QJsonArray& values ) const;

	//! Signaled when cache has been refreshed.
	void refreshed();

	//! Signaled when the populated status has changed.
	void populatedChanged();

public slots:

	//! Requests the cache refresh.
	void requestRefresh();

	//! Sets the cache content from the given network reply.
	void setContentFrom( int cookie, QNetworkReply* reply );

	//! Requests one item to be fetched.
	void fetchOneItem( int cookie, int id );

	/**
	 * @brief requestRefresh Request refresh of the list.
	 * @param cookie The cookie that identifies the quest.
	 */
	void requestRefreshWithCookie( int cookie );


// Protected interface.
protected:

	//! Definition for cached item container.
	typedef QHash< int, int > CACHE_MAPPER;

	//! Override this to clear the satellite data when the cache contents is cleared.
	virtual void clearSatelliteDataNts() {}

	//! Override this to populate satellite data that after the cache contens has been refreshed.
	virtual void populateSatelliteDataNts() {}

	//! Accesses the cache.
	const CACHE_MAPPER& cacheMapperNts() const { return m_cache; }

	//! Access the actual data.
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
	int getNextCookieNts() const { return m_nextCookie++; }

// Private data.
private:

	// Static data that does not need protecion.		
	QString m_resource;  //!< The cached resource.
	QString m_fetchOne;  //!< Resource string for fetching one item.

	// Dynamic variables.
	bool m_populated;  //!< Set to true when the cache has been populated.
	CACHE_MAPPER m_cache;  //!> A collection of available items.
	QJsonArray m_data;  //!< The actual data received via REST API.
	mutable int m_nextCookie;  //!< Next cookie. Cookies are used to distuinginsh requests to fething individual items from each other.
	
};

#endif // STRUCTURECACHEBASE_H
