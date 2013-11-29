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

#ifndef VAULTCORE_H
#define VAULTCORE_H

#include <QObject>
#include <QMutex>

#include "vaultcore.h"

// Forward declarations.
class ClassCache;
class ObjectTypeCache;
class PropertyDefCache;
class ObjectCache;

/**
 * @brief The VaultCore class
 *
 * Root class for caches associated with a vault.
 */
class VaultCore : public QObject
{
	Q_OBJECT
public:

	//! Constructor.
	explicit VaultCore(
		const QString& url,  //!< URL to the M-Files REST API.
		QObject *parent = 0  //!< Owner.
	);

	//! Destructor.
	virtual ~VaultCore();

	//! Authentication.
	QString authentication() const;

	//! Url
	QString url() const { return m_url; }

	//! Classes.
	ClassCache* classes() const { return m_classes; }

	//! Object types.
	ObjectTypeCache* objectTypes() const { return m_objectTypes; }

	//! Property definitions.
	PropertyDefCache* propertyDefinitions() const { return m_propertyDefinitions; }

	//! Objects.
	ObjectCache* objects() const { return m_objectCache; }
	
signals:

	//! Signaled the first time when all the caches have been populated.
	void allCachesPopulated();

	//! Authentication changed signal.
	void authenticationChanged( const QString& authentication );	
	
public slots:

	//! Updates the authentication info.
	void setAuthentication(
		const QString& authentication
	);

	//! A cache hosted by this core has been refreshed.
	void cacheRefreshed();

// Private data.
private:

	// Static variables that do not need protection.
	ClassCache* m_classes;
	ObjectTypeCache* m_objectTypes;
	PropertyDefCache*  m_propertyDefinitions;
	ObjectCache* m_objectCache;
	QString m_url;

	// Authentication information.
	mutable QMutex m_lock;	
	QString m_authentication;
	bool m_cachesPopulated;
	
};

#endif // VAULTCORE_H
