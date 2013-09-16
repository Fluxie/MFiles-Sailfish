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

#ifndef VAULTACCESS_H
#define VAULTACCESS_H

#include <QObject>
#include <QJsonValue>
#include <QMetaType>

// Forward declarations.
class VaultCore;

/*!
 * \brief The VaultCache class
 *
 * Provides a front for VaultCore and caches associated with it.
 * This object is owned by the UI thread so the signals and slots can be
 * associated with QML signals and slots.
 */
class VaultCache : public QObject
{
	Q_OBJECT
	Q_ENUMS( CacheType )
public:

	enum CacheType
	{
		Class,
		ObjectType
	};

	//! Constructor.
	explicit VaultCache(
		QObject *parent = 0
	);

	//! Destructor.
	virtual ~VaultCache();

	//! Initializes the vault cache.
	Q_INVOKABLE void initialize(
		const QString& url,  //!< M-Files Web Access URL
		const QString& authentication  //!< Authentication token for accessing API services.
	);

	//! Gets an item from the cache.
	Q_INVOKABLE QJsonValue get(
		CacheType type,  //!< The type of the requested item.
		int id  //!< The id of the requested item.
	) const;
	
signals:

	//! Signaled the first time when all the caches have been populated.
	void allCachesPopulated();

	//! Signaled when classes are refreshed.
	void classesRefreshed();

	//! Signaled when object types are refreshed.
	void objectTypesRefreshed();
	
public slots:
	
// Private data
private:

	VaultCore* m_core;  //!< Vault core.
};

#endif // VAULTACCESS_H
