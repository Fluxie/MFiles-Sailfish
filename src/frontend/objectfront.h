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

#ifndef OBJECTFRONT_H
#define OBJECTFRONT_H

#include <QObject>
#include <QJsonValue>
#include <QJsonObject>
#include <QSharedPointer>

#include "../backend/objectcore.h"

// Forward declarations.
class ObjectVersionFront;

class ObjectFront : public QObject
{
	Q_OBJECT
	Q_PROPERTY( int latestVersion READ latestVersion NOTIFY latestVersionChanged )

public:

	//! Instantiates new object front.
	explicit ObjectFront( QSharedPointer< ObjectCore >& core );

	//! ObjVer Json object this instance represents.
	Q_INVOKABLE QJsonValue id() const { return QJsonValue( m_id ); }

	//! ObjectVersion Json object this instance represents.
	Q_INVOKABLE QJsonValue objectVersion() const { return QJsonValue( m_objectVersion ); }

	//! Latest version.
	int latestVersion() const { return m_core->latestVersion(); }

	//! Latest object version.
	Q_INVOKABLE ObjectVersionFront* latest() const;

	//! Latest object version
	Q_INVOKABLE ObjectVersionFront* version(
		const QJsonValue& version
	) const;

signals:

	//! Emitted when the latest known version of this object has changed.
	void latestVersionChanged();

public slots:

	//! Requests the latest version to be fetched.
	void requestLatestVersion();

// Private data.
private:

	QJsonObject m_id;  //!< Object identity.
	QJsonObject m_objectVersion;  //!< Basic object data.
	mutable QSharedPointer< ObjectCore > m_core;  //!< The referred object.
	
};

#endif // OBJECTFRONT_H
