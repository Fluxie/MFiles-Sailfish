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

#include "objectversioncore.h"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QThread>

#include "mfwsrest.h"
#include "objectcore.h"
#include "vaultcore.h"

ObjectVersionCore::ObjectVersionCore( ObjectCore *parent, ObjVer objver ) :
	QObject( parent ),
	m_owner( parent ),
	m_rest( 0 ),
	m_objver( objver )
{	
	// Initialize.
	initialize();
}

//! Initializes the ObjectVersionCore
ObjectVersionCore::ObjectVersionCore(
	ObjectCore *parent,
	QJsonObject objectVersion
) :
	QObject( parent ),
	m_owner( parent ),
	m_rest( 0 ),
	m_objver( objectVersion ),
	m_objectVersion( objectVersion )
{
	// Initialize.
	initialize();
}

//! Sets object version.
void ObjectVersionCore::setObjectVersion(
	const QJsonObject& objectVersion
)
{
	// Protect access.
	{
		QMutexLocker lock( &m_mtx );

		// Properties for display.
		m_objectVersion = objectVersion;
	}

	// Object version information has changed.
	emit objectVersionChanged();
}

//! Sets the properties for display.
void ObjectVersionCore::setPropertiesForDisplay(
	const QJsonArray& propertiesForDisplay
)
{
	// Protect access.
	{
		QMutexLocker lock( &m_mtx );

		// Properties for display.
		m_propertiesForDisplay = propertiesForDisplay;
	}

	// Properties for display changed.
	emit propertiesForDisplayChanged();
}

//! Sets the normal property values.
void ObjectVersionCore::setProperties(
	const QJsonArray& properties
)
{
	// Protect access.
	{
		QMutexLocker lock( &m_mtx );

		// Properties.
		m_properties = properties;
	}

	// Properties changed.
	emit propertiesChanged();
}

//! Performs initialization operations.
void ObjectVersionCore::initialize()
{
	// We must be in the same thread as our owner.
	this->moveToThread( m_owner->thread() );

	// We can perform certain initialization only from the thread that will own this object.
	if( QThread::currentThread() == this->thread() )
	{
		// We can initiate the initializations immediately.
		if( m_objectVersion.empty() )
			this->refreshObjectVersion();
		this->refreshPropertyValues();
	}
	else
	{
		// Delay the initializations.
		if( m_objectVersion.empty() )
			QMetaObject::invokeMethod( this, "refreshObjectVersion", Qt::QueuedConnection );

		// Refresh property values.
		QMetaObject::invokeMethod( this, "refreshPropertyValues", Qt::QueuedConnection );

	}  // end if
}

//! Requests the properties for display to be fetched.
void ObjectVersionCore::requestPropertiesForDisplay()
{
	// Fetch information about the latest version.
	QString resource( "/objects/%1/%2/%3/properties?forDisplay=true" );
	QString args = resource.arg( m_objver.type() ).arg( m_objver.id() ).arg( m_objver.version() );
	QNetworkReply* reply = this->rest()->getJson( args );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {

		// Parse the returned JSON.
		QByteArray replyContent = reply->readAll();
		QJsonDocument result = QJsonDocument::fromJson( replyContent );
		QJsonArray propertiesForDisplay = result.array();

		// Store the properties for display.
		this->setPropertiesForDisplay( propertiesForDisplay );
	} );
}

// !Refreshes object version information.
void ObjectVersionCore::refreshObjectVersion()
{

}

//! Refreshes property value information.
void ObjectVersionCore::refreshPropertyValues()
{

}

//! Gets the REST API.
MfwsRest* ObjectVersionCore::rest()
{
	// Initialize the REST API if not already done.
	if( m_rest == 0 )
	{
		// Create the MfwsRest object for fetching the actual data.
		m_rest = new MfwsRest( m_owner->vault()->url(), this );
		m_rest->setAuthentication( m_owner->vault()->authentication() );
		QObject::connect( m_owner->vault(), &VaultCore::authenticationChanged, m_rest, &MfwsRest::setAuthentication );
	}
	return m_rest;
}

