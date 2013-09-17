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
#include <QJsonObject>
#include <QMutex>
#include <QHash>

// Foward declarations.
class VaultCore;
class MfwsRest;
class QNetworkReply;

class StructureCacheBase : public QObject
{
	//! Definition for cached item container.
	typedef QHash< int, QJsonObject >  CACHE_CONTAINER;

	Q_OBJECT
public:

	//! Constructor.
	explicit StructureCacheBase(
		const QString& resource,  //!< The resource this cache caches. E.g /structure/classes
		VaultCore *parent  //!< Parent vault.
	);

	//! Destructor.
	virtual ~StructureCacheBase() {}

	//! Gets an item from the cache.
	Q_INVOKABLE QJsonValue get( int id ) const;

	//! Is this cache populated?
	bool populated() const { return m_populated; }
	
signals:

	//! Signaled when cache is unable to server a request from the cache.
	void missing( int id ) const;

	//! Signaled when the previously missing item becomes available.
	void available( int id, QJsonObject* item ) const;

	//! Signaled when cache has been refreshed.
	void refreshed();

public slots:

	//! Requests the cache refresh.
	void requestRefresh();

	//! Sets the cache content from the given network reply.
	void setContentFrom( QNetworkReply* reply );

// Private data.
private:

	// Static data that does not need protecion.
	VaultCore* m_vault;
	MfwsRest* m_rest;  //!< Access to M-Files REST API.
	QString m_resource;  //!< The cached resource.

	mutable QMutex m_lock;  //!< Protects access to members.
	bool m_populated;  //!< Set to true when the cache has been populated.
	CACHE_CONTAINER m_cache;  //!> A collection of available items.
	
};

#endif // STRUCTURECACHEBASE_H
