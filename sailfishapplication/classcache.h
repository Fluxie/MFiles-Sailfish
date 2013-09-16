/*
 *  Copyright 2013 Juha Lepola
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

#ifndef CLASSCACHE_H
#define CLASSCACHE_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include <QJsonObject>


// Forward declarations.
class QNetworkReply;
class VaultCore;
class MfwsRest;

//! Cache for classes.
class ClassCache : public QObject
{
	typedef QHash< int, QJsonObject >  CLASS_CONTAINER;

	Q_OBJECT
public:

	//! Constructor
	explicit ClassCache( VaultCore *parent = 0);

	//! Gets class from the cache.
	Q_INVOKABLE QJsonValue get( int id ) const;

	//! Is this cache populated?
	bool populated() const { return m_populated; }
	
signals:

	//! Signaled when cache is unable to server a request from the cache.
	void missing( int id ) const;

	//! Signaled when the previously missing class becomes available.
	void available( int id, QJsonObject* mfilesClass ) const;

	//! Signaled when cache has been refreshed.
	void refreshed();
	
public slots:

	//! Attempts to cache the specified class.
	void tryFetch( int id );

	//! Requests the cache refresh.
	void requestRefresh();

	//! Sets the cache content from the given network reply.
	void setContentFrom( QNetworkReply* reply );

// Private data.
private:

	// Static data that does not need protecion.
	VaultCore *m_vault;
	MfwsRest *m_rest;  //!< Access to M-Files REST API.

	mutable QMutex m_lock;  //!< Protects access to members.
	bool m_populated;  //!< Set to true when the cache has been populated.
	CLASS_CONTAINER m_classes;  //!> A collection of available classes.
};

#endif // CLASSCACHE_H
