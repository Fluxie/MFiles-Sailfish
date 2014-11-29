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
#include <QNetworkReply>

#include "../errors/errorinfo.h"

// Forward declarations.
class ClassCache;
class ListingCache;
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

	/**
	 * @brief Initializes new VaultCore object.
	 * @param url URL to the M-Files REST API.
	 * @param authentication Authentication information if available.
	 * @param name The name of the vault.
	 * @param parent Parent object.
	 */
	explicit VaultCore( const QString& url,	const QString& authentication, const QString& name, QObject *parent = 0 );

	//! Destructor.
	virtual ~VaultCore();

	/**
	 * @brief Gets the name of the vault.
	 * @return Returns the name of the vault.
	 */
	QString name() const;

	/**
	 * @brief Gets the authentication information.
	 * @return Returns the authentication information.
	 */
	QString authentication() const;

	/**
	 * @brief Gets the URL to M-Files REST API.
	 * @return Returns the URL to M-Files REST API.
	 */
	QString url() const { return m_url; }

	/**
	 * @brief Gets the classes cache.
	 * @return Returns reference to the classes cache.
	 */
	ClassCache* classes() const { return m_classes; }

	/**
	 * @brief Gets the object type cache.
	 * @return Returns reference to the object type cache.
	 */
	ObjectTypeCache* objectTypes() const { return m_objectTypes; }

	/**
	 * @brief Gets the property definition cache.
	 * @return Returns reference to the property definition cache.
	 */
	PropertyDefCache* propertyDefinitions() const { return m_propertyDefinitions; }

	/**
	 * @brief Gets the object cache.
	 * @return Returns reference to the object cache.
	 */
	ObjectCache* objects() const { return m_objectCache; }

	/**
	 * @brief Gets the listing cache.
	 * @return Returns reference to the listing cache.
	 */
	ListingCache* listings() const { return m_listingCache; }
	
signals:

	/**
	 * @brief This signal is emitted when all the caches have been populated for the first time.
	 */
	void allCachesPopulated();

	/**
	 * @brief This signal is emitted when the authentication information has changed.
	 * @param authentication The new changed authentication information.
	 */
	void authenticationChanged( const QString& authentication );

	/**
	 * @brief This signal is emitted when an error occurss within the vault's "core" operation.
	 * @param error Description of the error.
	 */
	void error( const ErrorInfo& error );
	
public slots:

	/**
	 * @brief Updates the authenticaion information
	 * @param authentication Updated authentication information.
	 */
	void setAuthentication( const QString& authentication );

	/**
	 * @brief Reports an error that has occurred within the vault.
	 * @param errorinfo Description of the error.
	 */
	void reportError( const ErrorInfo& errorinfo );

private slots:

	/**
	 * @brief Notifies the vault that a cache has been refreshed.
	 */
	void cacheRefreshed();

// Private data.
private:

	// Static variables that do not need protection.
	QString m_name;
	ClassCache* m_classes;
	ObjectTypeCache* m_objectTypes;
	PropertyDefCache*  m_propertyDefinitions;
	ObjectCache* m_objectCache;
	ListingCache* m_listingCache;
	QString m_url;

	// Authentication information.
	mutable QMutex m_lock;	
	QString m_authentication;
	bool m_cachesPopulated;
	
};

#endif // VAULTCORE_H
