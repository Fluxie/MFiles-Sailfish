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

#ifndef HOSTCORE_H
#define HOSTCORE_H

#include <QThread>

#include "../errorinfo.h"

// Forward declarations.
class AppMonitor;
class VaultCore;

class HostCore : public QThread
{
    Q_OBJECT
public:

	//! Constructor.
	explicit HostCore( AppMonitor* monitor );

	//! Returns Host instance.
	static HostCore* instance();
    
signals:

	//! Signaled when an error occurss within the application.
	void error( const ErrorInfo& error );
    
public slots:

	//! Prepares vault.
	VaultCore* prepareVault(
		const QString& url,
		const QString& authentication
	);

	//! Reports an error somewhere within the application.
	void reportError( const ErrorInfo& error );

// Private interface.
private:

	//! Implementation.
	virtual void run();

// Private data.
private:

	AppMonitor* m_monitor;  //!< Global application monitor.

};

#endif // HOSTCORE_H
