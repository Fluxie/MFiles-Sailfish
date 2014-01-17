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
#include <QJsonObject>
#include <QNetworkReply>
#include <QThread>
#include <QtAlgorithms>

#include "mfwsrest.h"
#include "objectcore.h"
#include "vaultcore.h"

ObjectVersionCore::ObjectVersionCore( ObjectCore *parent, ObjVer objver ) :
	CoreBase( parent->vault(), parent ),
	m_owner( parent ),
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
	CoreBase( parent->vault(), parent ),
	m_owner( parent ),
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
		m_propertiesForDisplay = ObjectVersionCore::normalizePropertyValues( ForDisplay, propertiesForDisplay );
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
		m_properties = ObjectVersionCore::normalizePropertyValues( All, properties );
	}

	// Properties changed.
	emit propertiesChanged();
}

/**
 * @brief Sends the given property values to the server. This creates a new version of the object.
 * @param updatedProperties Properties to send.
 */
void ObjectVersionCore::sendPropertiesToServer( const QJsonArray& updatedProperties )
{
	qDebug( "Sending properties to server." );

	// Acquire copy of the current property values.
	QJsonArray forSending;
	{
		QMutexLocker lock( &m_mtx );

		// Check that we actually have valid property values to work with.
		if( m_properties.size() == 0 || m_propertiesForDisplay.size() == 0 )
		{
			qCritical( "TODO: Report error - Must fetch properties before sending them back to the server." );
			return;
		}

		// Take the copy.
		forSending = m_properties;
	}

	// Map update property values by property definition to their location
	// in the input array.
	QHash< int, int > updatedPropertiesIndex;
	for( int i = 0; i < updatedProperties.size(); i++ )
	{
		// Map the current property value by property definition to the index.
		QJsonValue updated = updatedProperties[ i ];
		QJsonObject asObject = updated.toObject();
		int propertyDef = asObject[ "PropertyDef" ].toDouble();
		updatedPropertiesIndex.insert( propertyDef, i );
		qDebug( "Proeprty mapping." );

	}  // end for

	// TODO: Calculate removed properties based on the current for display properties

	// Update the property values with the client provided property values.
	for( QJsonArray::iterator itr = forSending.begin(); itr != forSending.end(); itr++ )
	{
		// Get the property definition of the current property value.
		QJsonValueRef current = (*itr);
		QJsonObject asObject = current.toObject();
		int propertyDef = asObject[ "PropertyDef" ].toDouble();

		// Try finding the property value from the set of updated property values and update it.
		QHash< int, int >::const_iterator itrUpdated = updatedPropertiesIndex.find( propertyDef );
		if( itrUpdated != updatedPropertiesIndex.end() )
		{
			// This property value has been updated.
			qDebug( "Updated property value." );
			current = updatedProperties[ itrUpdated.value() ];
		}

	}  // end for

	// Send the property values to the server.
	QString resource( "/objects/%1/%2/%3/properties?include=properties,propertiesForDisplay" );
	QString args = resource.arg( m_objver.type() ).arg( m_objver.id() ).arg( m_objver.version() );
	QJsonDocument document( forSending );
	QNetworkReply* reply = this->rest()->putJson( args, document );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
			this->m_owner->versionAvailable( reply, true ); } );
}

//! Performs initialization operations.
void ObjectVersionCore::initialize()
{
	// We must be in the same thread as our owner.
	this->moveToThread( m_owner->thread() );
	QObject::connect( this, &ObjectVersionCore::error, m_owner, &ObjectCore::reportError );

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
	qDebug( "TODO" );
}

//! Refreshes property value information.
void ObjectVersionCore::refreshPropertyValues()
{
	qDebug( "TODO" );
}

/** Property values that are blocked for display purposes.
 *
 * NOTE: MFWS REST API should not include the property values 81 and 82 in the propertiesForDisplay.
 *
 * 81 = Favorite View
 * 82 = Accessed by Me
 */
const int ObjectVersionCore::BLOCKED_FOR_DISPLAY_PROPERTY_VALUES[] = { 81, 82 };

/**
 * @brief Normalizes property values for use. Receives the normalized property values.
 * @param purpose The usage purpose of the property values.
 * @param values VAlues to normalize.
 * @return Normalized property values.
 */
QJsonArray ObjectVersionCore::normalizePropertyValues( PropertyValuePurpose purpose, const QJsonArray& values )
{
	// Normalize the property values for the intended purpose.
	QJsonArray normalized( values );
	for( QJsonArray::iterator itr = normalized.begin(); itr != normalized.end(); )
	{
		// Normalize the property value.
		if( normalizePropertyValue( purpose, *itr ) )
		{
			// The value is accepted. Move to the next value.
			itr++;
		}
		else
		{
			// The value was not accepted. Erase it.
			itr = normalized.erase( itr );
		}

	}  // end for.

	// Return the normalized values.
	return normalized;
}


//! Normalizes property value for use. Receives true if the property value is acceptable for the intended purpose.
bool ObjectVersionCore::normalizePropertyValue( PropertyValuePurpose purpose, QJsonValueRef value )
{
	// Block certain property values.
	QJsonObject asObject = value.toObject();
	switch( purpose )
	{
	case ForDisplay :
		{
			Q_ASSERT( asObject.contains( "PropertyDef" ) );
			int propertyDef = asObject[ "PropertyDef" ].toDouble();
			const int* blocked = qFind( BLOCKED_FOR_DISPLAY_PROPERTY_VALUES,
										BLOCKED_FOR_DISPLAY_PROPERTY_VALUES + sizeof( BLOCKED_FOR_DISPLAY_PROPERTY_VALUES ) / sizeof( int ),
										propertyDef );
			if( blocked != BLOCKED_FOR_DISPLAY_PROPERTY_VALUES + sizeof( BLOCKED_FOR_DISPLAY_PROPERTY_VALUES ) / sizeof( int ) )
				return false;  // The property value was blocked.
		}
		break;

	// All property values are accepted.
	case All:
		break;

	// Unexpected purpose.
	case Undefined :
	default :
		qCritical( "TODO: Error reporting." );
		return false;
		break;
	}

	// Normalize the typed value part.
	QJsonObject typedValue = asObject[ "TypedValue" ].toObject();
	if( ! typedValue.contains( "HasValue" ) )
		typedValue[ "HasValue" ] = false;  // MFWS REST API does not include the field "HasValue" if the value is undefined. This can be considered a bug in MFWS REST API.

	// Perform data type specific normalization.
	Q_ASSERT( typedValue.contains( "DataType" ) );
	int dataType = typedValue[ "DataType" ].toDouble();
	switch( dataType )
	{
	// Single-line text.
	case 1:
		{
			// For single-line text values we explicitly set the value to an empty string
			// if we receive an undefined or null text based value.
			QJsonValue value = typedValue[ "Value" ];
			if( ! typedValue.contains( "Value" ) ||  // "Value" is not included if the value in M-Files is empty/null. This is a bug in MFWS REST API.
				value.isUndefined() || value.isNull() )
			{
				// Ensure that the empty single-line text is represented by a valid string.
				// In M-Files null, undefined and empty string are semantically equivalent.
				typedValue[ "Value" ] = QString( "" );
				typedValue[ "DisplayValue" ] = QString( "" );
			}
		}
		break;

	// The data type does not require special handling.
	default:
		break;
	}
	// NOTE: QJsonXYZ objects use implicit sharing of the data. e.g. copies are only created when
	// something is modified. As we just modified the values and created copies we need to assign the copies
	// back to the original values.
	asObject[ "TypedValue" ] = typedValue;  // Re-assign the typed value to the PropertyVAlue object.
	value = asObject;  // Update the actual property value.

	// This property value was accepted.
	return true;
}
