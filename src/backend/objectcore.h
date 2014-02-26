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

#ifndef OBJECTCORE_H
#define OBJECTCORE_H

#include <QObject>
#include <QHash>
#include <QJsonValue>
#include <QMutex>

#include "corebase.h"
#include "../mfiles/objid.h"

// Forward declarations.
class ObjectVersionCore;
class MfwsRest;
class QNetworkReply;
class VaultCore;

/**
 * @brief The ObjectCore class
 */
class ObjectCore : public CoreBase
{
	Q_OBJECT

// Private types.
private:

	/**
	 * @brief Definition for object version storage.
	 */
	typedef QHash< int, ObjectVersionCore* > VERSIONS;

public:

	/**
	 * @brief Initializes new ObjectCore object.
	 * @param vault Vault of the
	 * @param id The object id of the object this object represents.
	 */
	explicit ObjectCore( VaultCore* vault, MFiles::ObjID id );

	/**
	 * @brief Gets the latest known version of the obect.
	 * @return Latest known version.
	 */
	int latestVersion() const { QMutexLocker lock( &m_mtx ); return m_latestKnownVersion; }

	/**
	 * @brief Get the latest known version.
	 * @return Reference to the representative of the latest known version.
	 */
	ObjectVersionCore* latest();

	/**
	 * @brief Gets the specified version of the object.
	 * @param version The version to get.
	 * @return Reference to the representative of the specified version.
	 */
	ObjectVersionCore* version(
		const QJsonValue& version
	);
	
signals:

	/**
	 * @brief This signal is emitted when the latest known version of this object has changed.
	 */
	void latestVersionChanged();
	
public slots:

	/**
	 * @brief Refreshes the latest version.
	 */
	void requestLatestVersion();

private slots:

	/**
	 * @brief Notifies that a REST request for fetching object version information has completed.
	 * @param reply Replyt from the server.
	 * @param latest True if this version currently known to be the latest version.
	 */
	void versionAvailable( QNetworkReply* reply, bool latest );

// Private interface.
private:

	/**
	 * @brief getCore
	 * @param version
	 * @return
	 */
	ObjectVersionCore* getCore( int version );

// Private data.
private:

	friend class ObjectVersionCore;

	// Variables that are never updated.	
	MFiles::ObjID m_id;  //!< The id of this object.

	mutable QMutex m_mtx;  //!< Mutex that protects this object.
	int m_latestKnownVersion;  //!< The latest known object version.
	bool m_latestVersionCached;  //!< True when the latest known version is cached in m_versions.
	VERSIONS m_versions;  //!< Object version information.
};

#endif // OBJECTCORE_H
