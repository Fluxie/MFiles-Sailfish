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

#include "hostcore.h"
#include "../errors/appmonitor.h"
#include "vaultcore.h"

#include <QEventLoop>
#include <QQmlEngine>

namespace
{
	static HostCore* g_instance;  //!< The one and only host core.
}



HostCore::HostCore( AppMonitor* monitor ) :
	QThread( 0 ),
	m_monitor( monitor )
{
	Q_ASSERT( g_instance == 0 );
	g_instance = this;

	// Connect events to the monitor.
	QObject::connect( this, &HostCore::error, monitor, &AppMonitor::reportError );
}

//! Returns Host instance.
HostCore* HostCore::instance()
{
	return g_instance;
}

/*!
 *Prepares vault.
 */
VaultCore* HostCore::prepareVault(
	const QString& url,
	const QString& authentication
)
{
	// Create a new vault object and move it to this thread.
	VaultCore* core = new VaultCore( url, authentication );
	core->moveToThread( this );
	QObject::connect( core, &VaultCore::error, this, &HostCore::reportError );
	QQmlEngine::setObjectOwnership( core, QQmlEngine::JavaScriptOwnership );

	// Return the vault core.
	return core;
}

//! Reports an error somewhere within the application.
void HostCore::reportError( const ErrorInfo& errorinfo )
{
	// Emit.
	PUSH_ERROR_LAYER( errorinfo );
	emit error( errorinfo );
}

void HostCore::run()
{
    // Start event loop.
    QEventLoop loop;
    int exitCode = loop.exec();
}
