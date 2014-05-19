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

#include "objectversionfront.h"

#include "../backend/objectversioncore.h"

ObjectVersionFront::ObjectVersionFront(
		QSharedPointer< ObjectCore >& objectCore,
		ObjectVersionCore* core
) :
	QObject(0),
	m_objVer( core->objver() ),
	m_objectCore( objectCore ),
	m_core( core )
{
	// Connect events.
	QObject::connect( m_core, &ObjectVersionCore::objectVersionChanged, this, &ObjectVersionFront::objectVersionChanged );
	QObject::connect( m_core, &ObjectVersionCore::propertiesForDisplayChanged, this, &ObjectVersionFront::propertiesForDisplayChanged );
	QObject::connect( m_core, &ObjectVersionCore::propertiesChanged, this, &ObjectVersionFront::propertiesChanged );
}

//! Gets ObjectVersion.
QJsonValue ObjectVersionFront::objectVersion() const { return m_core->objectVersion(); }

//! Gets property values for display.
QJsonValue ObjectVersionFront::propertiesForDisplay() const { return m_core->propertiesForDisplay(); }

//! Gets property values.
QJsonValue ObjectVersionFront::properties() const { return m_core->properties(); }

//! Object version.
const MFiles::ObjVer& ObjectVersionFront::objver() const
{
	//! Return the version info.
	return m_objVer;
}

//! Sends the given property values to the server. This creates a new version of the object.
void ObjectVersionFront::sendPropertiesToServer( const QJsonArray& properties )
{
	// Delegate.
	QMetaObject::invokeMethod( this->m_core, "sendPropertiesToServer", Q_ARG( const QJsonArray&, properties ) );
}
