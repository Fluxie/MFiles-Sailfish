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

#ifndef OBJECTVERSIONFRONT_H
#define OBJECTVERSIONFRONT_H

#include <QObject>
#include <QSharedPointer>
#include <QJsonArray>
#include <QJsonValue>

#include "objectcore.h"
#include "mfiles/objver.h"

// Forward declarations.
class ObjectVersionCore;

class ObjectVersionFront : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QJsonValue objectVersion READ objectVersion NOTIFY objectVersionChanged )
	Q_PROPERTY( QJsonValue propertiesForDisplay READ propertiesForDisplay NOTIFY propertiesForDisplayChanged )
	Q_PROPERTY( QJsonValue properties READ properties NOTIFY propertiesChanged )

public:

	//! Initializes the front.
	explicit ObjectVersionFront(
		QSharedPointer< ObjectCore >& objectCore,
		ObjectVersionCore* core
	);

	//! Gets ObjectVersion.
	QJsonValue objectVersion() const;

	//! Gets property values for display.
	QJsonValue propertiesForDisplay() const;

	//! Gets property values.
	QJsonValue properties() const;


// Public methods that are not exposed to QML.
public:

	//! Object version.
	const MFiles::ObjVer& objver() const;
	
signals:

	//! Signaled when object version information has changed.
	void objectVersionChanged();

	//! Signaled when the properties for display have changed.
	void propertiesForDisplayChanged();

	//! Signaled when the properties have changed.
	void propertiesChanged();
	
public slots:

	//! Sends the given property values to the server. This creates a new version of the object.
	void sendPropertiesToServer( const QJsonArray& properties );

// Private data.
private:

	MFiles::ObjVer m_objVer;
	QSharedPointer< ObjectCore > m_objectCore;
	ObjectVersionCore* m_core;
};

#endif // OBJECTVERSIONFRONT_H
