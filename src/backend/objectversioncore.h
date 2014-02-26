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
#include "../mfiles/objver.h"

// Forward declarations.
class MfwsRest;
class ObjectCore;

/**
 * @brief The ObjectVersionCore class represent a single object version of M-Files object.
 */
class ObjectVersionCore : public CoreBase
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new ObjectVersionCore object.
	 * @param parent Parent for this object.
	 * @param objver ObjVer of this version.
	 */
	explicit ObjectVersionCore( ObjectCore *parent, MFiles::ObjVer objver );

	/**
	 * @brief Initializes new ObjectVersionCore object.
	 * @param parent Parent for this object.
	 * @param objectVersion JSON object describing the object version.
	 * @see <a href="http://www.m-files.com/mfws/structs/objectversion.html">Object version</a> in M-Files REST API.
	 */
	explicit ObjectVersionCore(	ObjectCore *parent, QJsonObject objectVersion );

	/**
	 * @brief Gets the object version.
	 * @return Object version.
	 */
	QJsonValue objectVersion() const { QMutexLocker lock( &m_mtx ); return QJsonValue( m_objectVersion ); }

	/**
	 * @brief Gets the property values for display of this object version.
	 * @return Property values for display.
	 */
	QJsonValue propertiesForDisplay() const { QMutexLocker lock( &m_mtx ); return QJsonValue( m_propertiesForDisplay ); }

	/**
	 * @brief Gets the property values of this object version.
	 * @return Property values.
	 */
	QJsonValue properties() const { QMutexLocker lock( &m_mtx ); return QJsonValue( m_properties ); }

	/**
	 * @brief Gets the object version.
	 * @return ObjVer.
	 */
	MFiles::ObjVer objver() const  { QMutexLocker lock( &m_mtx ); return m_objver; }
	
signals:

	/**
	 * @brief This signal is emitted when object version information has changed.
	 */
	void objectVersionChanged();

	/**
	 * @brief This signal is emitted when the properties for display have changed.
	 */
	void propertiesForDisplayChanged();

	/**
	 * @brief This signal is emitted when the properties have changed.
	 */
	void propertiesChanged();
	
public slots:

	/**
	 * @brief Sets the properties for display.
	 * @param propertiesForDisplay The properties for display.
	 */
	void setPropertiesForDisplay( const QJsonArray& propertiesForDisplay );

	/**
	 * @brief Sets the property values.
	 * @param properties The property values.
	 */
	void setProperties( const QJsonArray& properties );

	/**
	 * @brief Requests the properties for display to be fetched and cached.
	 */
	void requestPropertiesForDisplay();

	/**
	 * @brief Sends the given property values to the server. This creates a new version of the object.
	 * @param properties The properties that are sent to the server.
	 */
	void sendPropertiesToServer( const QJsonArray& properties );

// Private interface.
private:

	/**
	 * @brief Performs common initialization operations.
	 */
	void initialize();

	/**
	 * @brief Refreshes object version information.
	 */
	void refreshObjectVersion();

	/**
	 * @brief Refreshes property value information.
	 */
	void refreshPropertyValues();

	/**
	 * @brief The intended usage purpose of the property value.
	 */
	enum PropertyValuePurpose
	{
		Undefined,  //!< Undefined.
		ForDisplay,  //!< Property value is ment for display.
		All  //!< All property values.
	};

	/**
	 * @brief Property values that are blocked for display purposes.
	 */
	static const int BLOCKED_FOR_DISPLAY_PROPERTY_VALUES[];

	/**
	 * @brief Normalizes property values for use. Receives the normalized property values.
	 * @param purpose The intended usage purpose of the property values.
	 * @param values Property values for normalization.
	 * @return Normalized property values.
	 */
	static QJsonArray normalizePropertyValues( PropertyValuePurpose purpose, const QJsonArray& values );

	/**
	 * @brief Normalizes property value for use. Receives true if the property value is acceptable for the intended purpose.
	 * @param purpose The intended usage purpose of the properyt value.
	 * @param value The property value to normalize.
	 * @return True if the value was normalized.
	 */
	static bool normalizePropertyValue( PropertyValuePurpose purpose, QJsonValueRef value );

private slots:

	/**
	 * @brief Sets object version.
	 * @param objectVersion
	 */
	void setObjectVersion( const QJsonObject& objectVersion	);

// Private data.
private:

	ObjectCore* m_owner;  //!< Object that owns this verision
	MFiles::ObjVer m_objver;  //!< Object version.

	mutable QMutex m_mtx;
	QJsonObject m_objectVersion;  //!< The basic object version data.
	QJsonArray m_propertiesForDisplay;  //!< Properties for display.
	QJsonArray m_properties;  //!< Property values.
	
};

#endif // OBJECTVERSIONCORE_H
