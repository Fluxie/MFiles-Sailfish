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
#include "vaultcore.h"

#include <QEventLoop>
#include <QQmlEngine>

namespace
{
	static HostCore* g_instance;  //!< The one and only host core.
}



HostCore::HostCore(QObject *parent) :
    QThread(parent)
{
	Q_ASSERT( g_instance == 0 );
	g_instance = this;
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
	const QString& url
)
{
	// Create a new vault object and move it to this thread.
	VaultCore* core = new VaultCore( url );
	core->moveToThread( this );
	QQmlEngine::setObjectOwnership( core, QQmlEngine::JavaScriptOwnership );

	// Return the vault core.
	return core;
}

/**
 * @brief HostCore::run
 *
 *
 */
void HostCore::run()
{
    // Start event loop.
    QEventLoop loop;
    int exitCode = loop.exec();
}
