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

#include "objectcore.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QThread>

#include "objectversioncore.h"
#include "mfwsrest.h"
#include "vaultcore.h"

ObjectCore::ObjectCore( VaultCore* vault, ObjID id ) :
	QObject( 0 ),
	m_vault( vault ),
	m_rest( 0 ),
	m_id( id ),
	m_latestKnownVersion( -1 ),
	m_latestVersionCached( false )
{
	// Must be in the same thread as VaultCore
	this->moveToThread( vault->thread() );

	// Request latest version.
	QMetaObject::invokeMethod( this, "requestLatestVersion", Qt::QueuedConnection );
}

//! Instructs the content of the latest version to be cached.
void ObjectCore::requestLatestVersion()
{
	// Start caching the latest version.
	ObjectVersionCore* latest = this->latest();
	if( latest == 0 )
	{
		// Fetch information about the latest version.
		QString resource("/objects/%1/%2/latest?include=properties,propertiesForDisplay");
		QString args = resource.arg( m_id.type() ).arg( m_id.id() );
		QNetworkReply* reply = this->rest()->getJson( args );
		QObject::connect( reply, &QNetworkReply::finished,  [=]() {
			this->versionAvailable( reply, true ); } );
	}	
}

//! Returns reference to the latest known version or null if the latest version is not known.
ObjectVersionCore* ObjectCore::latest()
{
	// Protect access.
	QMutexLocker lock( &m_mtx );

	// The latest version is not known.
	if( m_latestKnownVersion < 1 )		
		return 0;

	VERSIONS::iterator itr = m_versions.find( m_latestKnownVersion );
	if( itr != m_versions.end() )
	{
		// The core was already cached.
		return itr.value();
	}
	else
	{
		// This version has not been cached previously.
		ObjVer objver( m_id, m_latestKnownVersion );
		ObjectVersionCore* versionCore = new ObjectVersionCore( this, objver );
		m_versions.insert( objver.version(), versionCore );
		return versionCore;

	}  // end if
	return 0;
}

//! Returns reference to the specific version.
ObjectVersionCore* ObjectCore::version(
	const QJsonValue& version
)
{
	// Parse object verion.
	ObjVer specificVersion( version.toObject() );
	if( specificVersion.objId() != m_id )
		return 0;

	// TODO: Implement.
	return 0;
}

//! Accesses the vault. May return NULL.
VaultCore* ObjectCore::vault()
{

	// This can only be called from the thread that owns the *Core objects.
	// Otherwise in some peculiar situations the vault could have been destroyed
	// wihtout us detecting it.
	if( QThread::currentThread() != this->thread() )
	{
		qDebug( "ObjectCore: VaultCore thread access violation.");
	}

	// Return the vault.
	return m_vault;
}

//! Called when a REST request for fetching object version information becomes available.
void ObjectCore::versionAvailable( QNetworkReply* reply, bool latest )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	QJsonObject extendedVersion = result.object();

	// Populate the cache.
	bool latestKnownVersionChanged = false;
	{
		// Construct properties array and then remove it to rever the extended version to standard object version.
		QJsonArray properties = extendedVersion[ "Properties" ].toArray();
		extendedVersion.remove( "Properties" );
		QJsonArray propertiesForDisplay;
		if( extendedVersion.contains( "PropertiesForDisplay" ) )
		{
			propertiesForDisplay = extendedVersion[ "PropertiesForDisplay" ].toArray();
			extendedVersion.remove( "PropertiesForDisplay" );
		}
		QJsonObject objverObject = extendedVersion[ "ObjVer" ].toObject();
		int version = objverObject[ "Version" ].toDouble();

		// Store the new object.
		QMutexLocker lock( &m_mtx );

		// Was the latest known version changed?
		if( latest )
		{
			// Did the latest known version change?
			if( version != m_latestKnownVersion )
				latestKnownVersionChanged = true;
			m_latestKnownVersion = version;

		}  // end if

		// Create and cache the version core.
		ObjVer objver( m_id, version );
		ObjectVersionCore* versionCore = new ObjectVersionCore( this, objver );
		versionCore->setProperties( properties );
		if( ! propertiesForDisplay.empty() )
			versionCore->setPropertiesForDisplay( propertiesForDisplay );
		else
			versionCore->requestPropertiesForDisplay();
		m_versions.insert( objver.version(), versionCore );

	}  // end lock

	// Was latest version changed?
	if( latestKnownVersionChanged )
		emit latestVersionChanged();
}

//! Gets the REST API.
MfwsRest* ObjectCore::rest()
{
	// Initialize the REST API if not already done.
	if( m_rest == 0 )
	{
		// Create the MfwsRest object for fetching the actual data.
		m_rest = new MfwsRest( m_vault->url(), this );
		m_rest->setAuthentication( m_vault->authentication() );
		QObject::connect( m_vault, &VaultCore::authenticationChanged, m_rest, &MfwsRest::setAuthentication );
	}
	return m_rest;
}
