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

#ifndef OBJECTVERSIONCORE_H
#define OBJECTVERSIONCORE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMutex>

#include "corebase.h"
#include "objver.h"

// Forward declarations.
class MfwsRest;
class ObjectCore;

class ObjectVersionCore : public CoreBase
{
	Q_OBJECT
public:

	//! Initializes the ObjectVersionCore
	explicit ObjectVersionCore(
		ObjectCore *parent,
		ObjVer objver
	);

	//! Initializes the ObjectVersionCore
	explicit ObjectVersionCore(
		ObjectCore *parent,
		QJsonObject objectVersion
	);

	//! Gets ObjectVersion.
	QJsonValue objectVersion() const { QMutexLocker lock( &m_mtx ); return QJsonValue( m_objectVersion ); }

	//! Gets property values for display.
	QJsonValue propertiesForDisplay() const { QMutexLocker lock( &m_mtx ); return QJsonValue( m_propertiesForDisplay ); }

	//! Gets property values.
	QJsonValue properties() const { QMutexLocker lock( &m_mtx ); return QJsonValue( m_properties ); }
	
signals:

	//! Signaled when object version information has changed.
	void objectVersionChanged();

	//! Signaled when the properties for display have changed.
	void propertiesForDisplayChanged();

	//! Signaled when the properties have changed.
	void propertiesChanged();
	
public slots:

	//! Sets object version.
	void setObjectVersion(
		const QJsonObject& objectVersion
	);

	//! Sets the properties for display.
	void setPropertiesForDisplay(
		const QJsonArray& propertiesForDisplay
	);

	//! Requests the properties for display to be fetched.
	void requestPropertiesForDisplay();

	//! Sets the normal property values.
	void setProperties(
		const QJsonArray& properties
	);

	//! Sends the given property values to the server. This creates a new version of the object.
	void sendPropertiesToServer( const QJsonArray& properties );

// Private interface.
private:

	//! Performs initialization operations.
	void initialize();

	// !Refreshes object version information.
	void refreshObjectVersion();

	//! Refreshes property value information.
	void refreshPropertyValues();

	//! The intended purpose of the property value.
	enum PropertyValuePurpose
	{
		Undefined,  //!< Undefined.
		ForDisplay,  //!< Property value is ment for display.
		All  //!< All property values.
	};

	//! Property values that are blocked for display purposes.
	static const int BLOCKED_FOR_DISPLAY_PROPERTY_VALUES[];

	//! Normalizes property values for use. Receives the normalized property values.
	static QJsonArray normalizePropertyValues( PropertyValuePurpose purpose, const QJsonArray& values );

	//! Normalizes property value for use. Receives true if the property value is acceptable for the intended purpose.
	static bool normalizePropertyValue( PropertyValuePurpose purpose, QJsonValueRef value );

// Private data.
private:

	ObjectCore* m_owner;  //!< Object that owns this verision
	ObjVer m_objver;  //!< Object version.

	mutable QMutex m_mtx;
	QJsonObject m_objectVersion;  //!< The basic object version data.
	QJsonArray m_propertiesForDisplay;  //!< Properties for display.
	QJsonArray m_properties;  //!< Property values.
	
};

#endif // OBJECTVERSIONCORE_H
