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
#include "../mfwsrest.h"
#include "vaultcore.h"

ObjectCore::ObjectCore( VaultCore* vault, const MFiles::ObjID& id ) :
	CoreBase( vault, 0 ),
	m_id( id ),
	m_latestKnownVersion( -1 ),
	m_latestVersionCached( false )
{
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

	// Get core.
	return this->getCore( m_latestKnownVersion );
}

//! Returns reference to the specific version.
ObjectVersionCore* ObjectCore::version(
	const QJsonValue& version
)
{
	// Parse object verion.
	int versionNumber = 0;
	if( version.isDouble() )
	{
		// As number.
		versionNumber = version.toDouble();
	}
	else if( version.isObject() )
	{
		// Parse from the ObjVer.
		MFiles::ObjVer specificVersion( version.toObject() );
		if( specificVersion.objId() != m_id )
		{
			qCritical( "Invalid specific version." );
			return 0;
		}
	}
	else if( version.isString() )
	{
		// Try parsing the version from string.
		QString asString = version.toString();
		bool conversionOk = false;
		versionNumber = asString.toInt( &conversionOk );
		if( !conversionOk )
		{
			qCritical() << QString( "Unexpected version %1." ).arg( asString );
			return 0;
		}
	}
	else
	{
		// Unexpected version.
		qCritical() << QString( "Unexpected version %1." ).arg( version.toString() );
		return 0;

	}  // end if

	// Is version number invalid at this point?
	if( versionNumber < 1 )
		return 0;

	// Return core.
	return this->getCore( versionNumber );
}

//! Called when a REST request for fetching object version information becomes available.
void ObjectCore::versionAvailable( QNetworkReply* reply, bool latest )
{
	// Process only if there was no error.
	if( reply->error() != QNetworkReply::NoError )
	{
		qDebug( "Error when sending the property values." );
		return;
	}

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
		MFiles::ObjVer objver( m_id, version );
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

//! Gets new core for the specified version.
ObjectVersionCore* ObjectCore::getCore( int version )
{
	// Try fetching a cached core and if not available
	// then return a new one.
	VERSIONS::iterator itr = m_versions.find( version );
	if( itr != m_versions.end() )
	{
		// The core was already cached.
		return itr.value();
	}
	else
	{
		// This version has not been cached previously.
		MFiles::ObjVer objver( m_id, version );
		ObjectVersionCore* versionCore = new ObjectVersionCore( this, objver );
		m_versions.insert( objver.version(), versionCore );
		return versionCore;

	}  // end if
	return 0;
}
