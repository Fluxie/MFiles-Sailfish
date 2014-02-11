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
#include "mfiles/objid.h"

// Forward declarations.
class ObjectVersionCore;
class MfwsRest;
class QNetworkReply;
class VaultCore;

class ObjectCore : public CoreBase
{
	Q_OBJECT

// Private types.
private:

	//!
	typedef QHash< int, ObjectVersionCore* > VERSIONS;

public:

	//! Initializes new ObjectCore.
	explicit ObjectCore( VaultCore* vault, ObjID id );

	//! Latest version.
	int latestVersion() const { QMutexLocker lock( &m_mtx ); return m_latestKnownVersion; }

	//! Returns reference to the latest known version or null if the latest version is not known.
	ObjectVersionCore* latest();

	//! Returns reference to the specific version.
	ObjectVersionCore* version(
		const QJsonValue& version
	);
	
signals:

	//! Signaled when the latest known version of this object has changed.
	void latestVersionChanged();
	
public slots:

	//! Instructs the content of the latest version to be cached.
	void requestLatestVersion();

	//! Called when a REST request for fetching object version information becomes available.
	void versionAvailable( QNetworkReply* reply, bool latest );

// Private interface.
private:

	//! Gets core for the specified version.
	ObjectVersionCore* getCore( int version );

// Private data.
private:

	friend class ObjectVersionCore;

	// Variables that are never updated.	
	ObjID m_id;  //!< The id of this object.

	mutable QMutex m_mtx;  //!< Mutex that protects this object.
	int m_latestKnownVersion;  //!< The latest known object version.
	bool m_latestVersionCached;  //!< True when the latest known version is cached in m_versions.
	VERSIONS m_versions;  //!< Object version information.
};

#endif // OBJECTCORE_H
