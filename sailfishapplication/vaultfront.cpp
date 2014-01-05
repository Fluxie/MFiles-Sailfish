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
#include <QQmlEngine>

#include "appmonitor.h"
#include "errorinfo.h"
#include "hostcore.h"
#include "vaultcore.h"
#include "classcache.h"
#include "objecttypecache.h"
#include "objectcache.h"
#include "objectfront.h"
#include "objver.h"
#include "propertydefcache.h"
#include "valuelistcore.h"
#include "valuelistfront.h"

VaultFront::VaultFront(QObject *parent) :
	QObject(parent),
	m_core( 0 )
{
}

//! Destructor.
VaultFront::~VaultFront()
{
	// Mark the associated vault core for destruction.
	if( m_core != 0 )
		m_core->deleteLater();
}

/**
 * @brief VaultFront::initialize
 * @param url
 * @param authentication
 */
void VaultFront::initialize(
		const QString& url,
		const QString& authentication )
{
	Q_ASSERT( m_core == 0 );

	// Prepare new core.
	m_core = HostCore::instance()->prepareVault( url, authentication );

	// Connect events
	QObject::connect( m_core, &VaultCore::allCachesPopulated, this, &VaultFront::allCachesPopulated, Qt::QueuedConnection );
	QObject::connect( m_core->classes(), &ClassCache::refreshed, this, &VaultFront::classesRefreshed, Qt::QueuedConnection );
	QObject::connect( m_core->objectTypes(), &ObjectTypeCache::refreshed, this, &VaultFront::objectTypesRefreshed, Qt::QueuedConnection );
	QObject::connect( m_core->propertyDefinitions(), &PropertyDefCache::populatedChanged, this, &VaultFront::propertyDefinitionsReadyChanged, Qt::QueuedConnection );
	QObject::connect( m_core->propertyDefinitions(), &PropertyDefCache::refreshed, this, &VaultFront::propertyDefinitionsRefreshed, Qt::QueuedConnection );
}

//! Gets an item from the cache.
QJsonValue VaultFront::get(
	CacheType type,  //!< The type of the requested item.
	int id
) const
{
	// Try fetching the item.
	QJsonValue item = QJsonValue::Null;
	switch( type )
	{
	case CacheType::Class :
		item = m_core->classes()->get( id );
		break;

	case CacheType::ObjectType :
		item = m_core->objectTypes()->get( id );
		break;

	case CacheType::PropertyDefinition :
		item = m_core->propertyDefinitions()->get( id );

	// Unknown item, return the null we set earlier.
	default:
		break;
	};

	// Return.
	return item;
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
		ObjVer objver( objverJson.toObject() );
		front = this->newFront( objver.objId() );
	}
	else if( objectInfo.find( QString( "Type" ) ) != objectInfo.end() &&
			 objectInfo.find( QString( "Version" ) ) == objectInfo.end() )
	{
		// This is ObjID Json object.
		ObjID objid( objectInfo );
		front = this->newFront( objid );
	}
	else if( objectInfo.find( QString( "Type" ) ) != objectInfo.end() &&
			 objectInfo.find( QString( "Version" ) ) != objectInfo.end() )
	{
		// This is ObjVer Json object.
		ObjVer objver( objectInfo );
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
	int propertyDefinition  //!< The id of the property definition used to filter the value list.
)
{
	// Value list requested.
	qDebug( QString( "Value list %1 requested for property definition %2." ).arg( id ).arg( propertyDefinition ).toLatin1() );

	// Try fetching available core.
	qDebug( "Fetching value list from object type cache." );
	ValueListCore* listCore = 0;
	if( propertyDefinition != -1 )
		listCore = m_core->objectTypes()->list( id, propertyDefinition );
	else
		listCore = m_core->objectTypes()->list( id );
	if( listCore == 0 )
		return 0;

	// Instantiate new front.
	qDebug( "Instantiating value list front." );
	ValueListFront* front = new ValueListFront( m_core, listCore );
	QQmlEngine::setObjectOwnership( front, QQmlEngine::JavaScriptOwnership );
	QObject::connect( m_core->objectTypes(), &ObjectTypeCache::valueListAvailable, front, &ValueListFront::coreAvailable );	

	// Return the front.
	qDebug( "Returning..." );
	return front;
}

//! Checks if the property definitions are ready.
bool VaultFront::propertyDefinitionsReady()
{
	// The property definitions are ready when the cache is populated.
	return m_core->propertyDefinitions()->populated();
}


//! Call when a new object has been changed.
void VaultFront::objectChanged(	const QJsonValue& objectInfo )
{

}


//! Gets new object front for the given object version.
ObjectFront* VaultFront::newFront( const ObjID& objid )
{
	// Create a new front for the given object core.
	QSharedPointer< ObjectCore > core = m_core->objects()->object( objid );
	ObjectFront* front = new ObjectFront( core );
	QObject::connect( core.data(), &ObjectCore::latestVersionChanged, front, &ObjectFront::latestVersionChanged, Qt::QueuedConnection );	
	return front;
}
