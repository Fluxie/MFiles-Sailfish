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

#include "objectfront.h"

#include <QQmlEngine>

#include "objver.h"
#include "objectversionfront.h"

ObjectFront::ObjectFront( QSharedPointer< ObjectCore >& core ) :
	QObject( 0 ),
	m_core( core )
{
	// Connect signals.
	QObject::connect( m_core.data(), &ObjectCore::latestVersionChanged, this, &ObjectFront::latestVersionChanged, Qt::QueuedConnection );
}

//! Latest object version.
ObjectVersionFront* ObjectFront::latest() const
{
	// Fetch the version core that represents the latest version.
	ObjectVersionCore* latestCore = m_core->latest();
	if( latestCore == 0 )
		return 0;

	// Return front for the core.
	ObjectVersionFront* front = new ObjectVersionFront( m_core, latestCore );
	QQmlEngine::setObjectOwnership( front, QQmlEngine::JavaScriptOwnership );
	return front;
}

//! Latest object version.
ObjectVersionFront* ObjectFront::version(
	const QJsonValue& version
) const
{
	// Fetch core.
	ObjectVersionCore* versionCore = m_core->version( version );
	if( versionCore == 0 )
		return 0;

	// Return front for the core.
	ObjectVersionFront* front = new ObjectVersionFront( m_core, versionCore );
	QQmlEngine::setObjectOwnership( front, QQmlEngine::JavaScriptOwnership );
	return front;
}

//! Requests the latest version to be fetched.
void ObjectFront::requestLatestVersion()
{
	// Delegate to the core.
	m_core->requestLatestVersion();
}
