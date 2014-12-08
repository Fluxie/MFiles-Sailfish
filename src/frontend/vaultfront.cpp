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

#include "vaultfront.h"

#include <QJsonValue>
#include <QSharedPointer>
#include <QQmlEngine>

#include "../errors/appmonitor.h"
#include "../backend/asyncfetch.h"
#include "classesfront.h"
#include "../errors/errorinfo.h"
#include "../backend/hostcore.h"
#include "../mfiles/mfilesconstants.h"
#include "../backend/vaultcore.h"
#include "../backend/classcache.h"
#include "../backend/listingid.h"
#include "../backend/listingcache.h"
#include "../backend/objecttypecache.h"
#include "../backend/objectcache.h"
#include "objectfront.h"
#include "../mfiles/objver.h"
#include "../backend/propertydefcache.h"
#include "../backend/valuelistcore.h"
#include "../backend/workflowstatescache.h"
#include "valuelistfront.h"
#include "workflowstatesfront.h"
#include "listingfront.h"

VaultFront::VaultFront( QObject *parent ) :
	FrontBase( nullptr, parent )
{
}

//! Destructor.
VaultFront::~VaultFront()
{
	// Mark the associated vault core for destruction.
	if( core() != nullptr )
		core()->deleteLater();
}

/**
 * @brief VaultFront::initialize
 * @param url
 * @param authentication
 */
void VaultFront::initialize(
		const QString& url,
		const QString& authentication,
		const QString& name )
{	
	Q_ASSERT( core() == nullptr );

	// Prepare new core.
	this->setCore( HostCore::instance()->prepareVault( url, authentication, name ) );

	// Connect events
	VaultCore* core = vaultCore();
	QObject::connect( core, &VaultCore::allCachesPopulated, this, &VaultFront::allCachesPopulated, Qt::QueuedConnection );
	QObject::connect( core->classes(), &ClassCache::populatedChanged, this, &VaultFront::classesReadyChanged, Qt::QueuedConnection );
	QObject::connect( core->classes(), &ClassCache::refreshed, this, &VaultFront::classesRefreshed, Qt::QueuedConnection );
	QObject::connect( core->objectTypes(), &ObjectTypeCache::refreshed, this, &VaultFront::objectTypesRefreshed, Qt::QueuedConnection );
	QObject::connect( core->propertyDefinitions(), &PropertyDefCache::populatedChanged, this, &VaultFront::propertyDefinitionsReadyChanged, Qt::QueuedConnection );
	QObject::connect( core->propertyDefinitions(), &PropertyDefCache::refreshed, this, &VaultFront::propertyDefinitionsRefreshed, Qt::QueuedConnection );

	// Authenticated.
	emit rootListingChanged();
	emit authenticatedChanged();
	emit nameChanged();
}

/**
 * @brief Gets the name of the vault.
 * @return The name of the vault.
 */
QString VaultFront::name() const
{
	// No core, no listing.
	if( coreConst() == nullptr )
		return "";

	// Delegate.
	return this->vaultCoreConst()->name();
}

//! Gets an item from the cache.
QJsonValue VaultFront::get(
	CacheType type,  //!< The type of the requested item.
	int id
) const
{
	// Try fetching the item.
	AsyncFetch* fetch = 0;
	switch( type )
	{
	case CacheType::Class :
		fetch = vaultCoreConst()->classes()->get( id );
		break;

	case CacheType::ObjectType :
		fetch = vaultCoreConst()->objectTypes()->get( id );
		break;

	case CacheType::PropertyDefinition :
		fetch = vaultCoreConst()->propertyDefinitions()->get( id );

	// Unknown item, return the null we set earlier.
	default:
		break;
	};	

	QJsonValue value;
	if( fetch->state() == AsyncFetch::Finished )
	{
		value = fetch->value();
		Q_ASSERT( ! value.isNull() && ! value.isUndefined() );
	}
	else if( fetch->state() == AsyncFetch::Error )
	{
		value = QJsonValue::Null;
	}
	else
	{
		Q_ASSERT( false );
	}
	fetch->deleteLater();

	// Return the value.
	return value;
}

//! Gets a reference to an object.
ObjectFront* VaultFront::object(
	const QJsonValue& objectInfoValue  //!< Json object that identifies the object. This can be ObjVer or ObjectVersion.
)
{
	// First identiy which type of id we have and
	// then get new front for the object.
	QJsonObject objectInfo = objectInfoValue.toObject();
	ObjectFront* front = 0;
	if( objectInfo.find( QString( "Title" ) ) != objectInfo.end() )
	{
		// This is an ObjectVersion Json object.
		QJsonValue objverJson = objectInfo[ "ObjVer" ];
		MFiles::ObjVer objver( objverJson.toObject() );
		front = this->newFront( objver.objId() );
	}
	else if( objectInfo.find( QString( "Type" ) ) != objectInfo.end() &&
			 objectInfo.find( QString( "Version" ) ) == objectInfo.end() )
	{
		// This is ObjID Json object.
		MFiles::ObjID objid( objectInfo );
		front = this->newFront( objid );
	}
	else if( objectInfo.find( QString( "Type" ) ) != objectInfo.end() &&
			 objectInfo.find( QString( "Version" ) ) != objectInfo.end() )
	{
		// This is ObjVer Json object.
		MFiles::ObjVer objver( objectInfo );
		front = this->newFront( objver.objId() );
	}
	else
	{
		// TODO: Error handling.
		front = 0;
	}

	// Set the ownership to JavaScript.
	if( front )
		QQmlEngine::setObjectOwnership( front, QQmlEngine::JavaScriptOwnership );

	// Return the front.
	return front;
}

//! Gets a reference to a value list.
ValueListFront* VaultFront::valueList(
	int id,  //!< The id of the requested value list.
	TypedValueFilter* filter  //!< Filter for searching value list items from the server.
)
{
	// Value list requested.

	// Try fetching available core.
	qDebug( "Fetching value list from object type cache." );
	ValueListCore* listCore = 0;
	if( filter && filter->enabled() )
		listCore = vaultCore()->objectTypes()->list( id, filter );
	else
		listCore = vaultCore()->objectTypes()->list( id );
	if( listCore == nullptr )
		return nullptr;

	// Instantiate new front.
	qDebug( "Instantiating value list front." );
	ValueListFront* front = 0;
	int objectType = MFiles::Constants::AllObjectTypes;  // All object types.
	if( filter && filter->enabled() )
		objectType = filter->objectType();
	switch( id )
	{
	// A special handler for classes value list.
	case MFiles::Constants::ClassesValueList :
		front = new ClassesFront( vaultCore(), listCore, objectType );
		break;

	case MFiles::Constants::WorkflowStatesValueList :
		front = newWorkflowStatesFront( listCore, filter );
		break;

	// Use the normal front by default.
	default:
		front = new ValueListFront( vaultCore(), listCore );
		break;
	}
	QQmlEngine::setObjectOwnership( front, QQmlEngine::JavaScriptOwnership );
	QObject::connect( vaultCore()->objectTypes(), &ObjectTypeCache::valueListAvailable, front, &ValueListFront::coreAvailable );

	// Return the front.	
	return front;
}

ListingFront* VaultFront::listing( const QString& path )
{
	// No core, no listing.
	if( core() == nullptr )
		return nullptr;

	// Return the requested listing.
	ListingFront* front = new ListingFront( vaultCore()->listings()->listing( ListingId( path ) ) );
	return front;
}

ListingFront* VaultFront::rootListing()
{
	// No core, no listing.
	if( core() == nullptr )
		return nullptr;

	// Return the root listing.
	ListingFront* front = new ListingFront( vaultCore()->listings()->root() );
	return front;
}

bool VaultFront::authenticated() const
{
	return coreConst() != nullptr;
}

//! Checks if the classes are ready.
bool VaultFront::classesReady() const
{
	// The classes are ready when the cache is populated.
	return vaultCoreConst()->classes()->populated();
}

//! Checks if the property definitions are ready.
bool VaultFront::propertyDefinitionsReady() const
{
	// The property definitions are ready when the cache is populated.
	return vaultCoreConst()->propertyDefinitions()->populated();
}


//! Call when a new object has been changed.
void VaultFront::objectChanged(	const QJsonValue& objectInfo )
{

}


//! Gets new object front for the given object version.
ObjectFront* VaultFront::newFront( const MFiles::ObjID& objid )
{
	// Create a new front for the given object core.
	QSharedPointer< ObjectCore > objectCore = vaultCore()->objects()->object( objid );
	ObjectFront* front = new ObjectFront( objectCore );
	QObject::connect( objectCore.data(), &ObjectCore::latestVersionChanged, front, &ObjectFront::latestVersionChanged, Qt::QueuedConnection );
	return front;
}

/**
 * @brief Accesses the vault core.
 * @return Vault core
 */
VaultCore* VaultFront::vaultCore()
{
	QObject* coreObject = core();
	if( coreObject == nullptr )
		return nullptr;

	return qobject_cast< VaultCore* >( coreObject );
}

/**
 * @brief Accesses the vault core.
 * @return Vautl core.
 */
const VaultCore* VaultFront::vaultCoreConst() const
{
	const QObject* core = coreConst();
	if( core == nullptr )
		return nullptr;

	return qobject_cast< const VaultCore* >( core );
}

ValueListFront* VaultFront::newWorkflowStatesFront( ValueListCore* statesList, TypedValueFilter* filter )
{
	// Try locating workflow states.
	WorkflowStatesCore* states = nullptr;
	if( filter != nullptr && filter->propertyDef() == MFiles::Constants::WorkflowStatesPropertyDef )
	{
		// Filter specified?
		MFiles::TypedValue owner( filter->ownerInfo() );
		QJsonArray owners = owner.asLookups();
		Q_ASSERT( owners.count() == 1 );
		MFiles::Lookup ownerLookup( owners[ 0 ] );
		MFiles::TypedValue currentState( filter->currentValue() );
		Q_ASSERT( currentState.dataType() == MFiles::Constants::Lookup );
		if( ! owner.isUndefined() && ! currentState.isUndefined() )
			states = vaultCoreConst()->workflowStates()->getStates( ownerLookup.item(), currentState.lookup().item() );

	}  // end if.

	// Return.
	return new WorkflowStatesFront( vaultCore(), statesList, states );
}
