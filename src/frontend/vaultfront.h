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

#include "../backend/typedvaluefilter.h"

#include "frontbase.h"

// Forward declarations.
namespace MFiles { class ObjID; }
class ListingFront;
class ObjectFront;
class ValueListCore;
class ValueListFront;
class VaultCore;
class AppMonitor;

/*!
 * \brief The VaultCache class
 *
 * Provides a front for VaultCore and caches associated with it.
 * This object is owned by the UI thread so the signals and slots can be
 * associated with QML signals and slots.
 */
class VaultFront : public FrontBase
{
	Q_OBJECT
	Q_ENUMS( CacheType )
	Q_PROPERTY( QString name READ name NOTIFY nameChanged )
	Q_PROPERTY( bool authenticated READ authenticated NOTIFY authenticatedChanged )
	Q_PROPERTY( bool classesReady READ classesReady NOTIFY classesReadyChanged )
	Q_PROPERTY( bool propertyDefinitionsReady READ propertyDefinitionsReady NOTIFY propertyDefinitionsReadyChanged )
	Q_PROPERTY( ListingFront* rootListing READ rootListing NOTIFY rootListingChanged )
public:

	enum CacheType
	{
		Class,
		ObjectType,
		PropertyDefinition
	};

	//! Constructor.
	explicit VaultFront(
		QObject *parent = 0
	);

	//! Destructor.
	virtual ~VaultFront();

	/**
	 * @brief Gets the name of the vault.
	 * @return The name of the vault.
	 */
	QString name() const;

	/**
	 * @brief Initializes the vault.
	 * @param url URL of the vault
	 * @param authentication Authentication / access information to the vault.
	 * @param name  The name of the vault
	 */
	Q_INVOKABLE void initialize(
		const QString& url,
		const QString& authentication,
		const QString& name
	);

	//! Gets an item from the cache.
	Q_INVOKABLE QJsonValue get(
		CacheType type,  //!< The type of the requested item.
		int id  //!< The id of the requested item.
	) const;

	//! Gets a reference to an object.
	Q_INVOKABLE ObjectFront* object(
		const QJsonValue& id  //!< Json object that identifies the object. This can be ObjVer or ObjectVersion.
	);

    //! Gets a reference to a value list.
    Q_INVOKABLE ValueListFront* valueList(
        int id  //!< The id of the requested value list.
			) { return valueList( id, 0 ); }

    //! Gets a reference to a value list.
    Q_INVOKABLE ValueListFront* valueList(
        int id,  //!< The id of the requested value list.
		TypedValueFilter* filter  //!< Filter for searching value list items from the server.
	);

	/**
	 * @brief Gets a listing of the specified path.
	 * @param path Path to the listing.
	 * @return A listing represented by the path.
	 */
	Q_INVOKABLE ListingFront* listing( const QString& path );

	/**
	 * @brief Gets the root listing.
	 * @return The root listing.
	 */
	ListingFront* rootListing();

	/**
	 * @brief Checks if this vault object has an authenticated access to the vault.
	 * @return True if this vault object has an authenticated access to a vault.
	 */
	bool authenticated() const;

	//! Checks if the classes are ready.
	bool classesReady() const;

	//! Checks if the property definitions are ready.
	bool propertyDefinitionsReady() const;
	
signals:

	/**
	 * @brief nameChanged is signaled when the name of the vault changes.
	 */
	void nameChanged();

	/**
	 * @brief authenticatedChanged is signaled when the vault gains or loses access to the vault.
	 */
	void authenticatedChanged();

	//! Signaled the first time when all the caches have been populated.
	void allCachesPopulated();

	//! Signaled when classes are refreshed.
	void classesRefreshed();

	//! Signaled when the classes read state is changed.
	void classesReadyChanged();

	//! Signaled when object types are refreshed.
	void objectTypesRefreshed();

	//! Signaled when property definitions are refreshed.
	void propertyDefinitionsRefreshed();

	//! Signaled when property definitions ready state is changed.
	void propertyDefinitionsReadyChanged();

	/**
	 * @brief Signaled when rootViewListingReady changes.
	 */
	void rootListingChanged() const;
	
public slots:

	//! Call when a new object has been changed.
	void objectChanged(	const QJsonValue& objectInfo );

// FrontBase:
protected:

	//! Does this front accept the specified core.
	virtual bool accept( QObject* core ) const { Q_UNUSED( core ); return false; }

// Private interface.
private:

	//! Gets new object front for the given object id.
	ObjectFront* newFront( const MFiles::ObjID& objid );

	/**
	 * @brief Accesses the vault core.
	 * @return Vault core
	 */
	VaultCore* vaultCore();

	/**
	 * @brief Accesses the vault core.
	 * @return Vautl core.
	 */
	const VaultCore* vaultCoreConst() const;

	/**
	 * @brief newWorkflowStatesFront
	 * @param statesList Workflow states value list.
	 * @param filter Filter for searching value list items from the server.
	 * @return Workflow states list front.
	 */
	ValueListFront* newWorkflowStatesFront( ValueListCore* statesList, TypedValueFilter* filter  );

};

#endif // VAULTACCESS_H
