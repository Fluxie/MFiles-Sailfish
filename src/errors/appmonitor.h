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

#ifndef APPMONITOR_H
#define APPMONITOR_H

#include <QObject>
#include <QVector>

#include "errorinfo.h"

// Forward declarations.
class ErrorModel;

//! Application monitor for monitoring the status of the application for errors and other related information.
class AppMonitor : public QObject
{
	Q_OBJECT
	Q_PROPERTY( ErrorModel* lastError READ lastError NOTIFY lastErrorChanged )
public:

	//! Initializes the AppMonitor.
	explicit AppMonitor( QObject *parent = 0 );

	//! Destructor.
	virtual ~AppMonitor();


	//! The last error messagge.
	ErrorModel* lastError() const { return m_lastError; }

	//! Hides all current errors from the user.
	Q_INVOKABLE void hideErrors();

signals:

	//! Signaled when the last error changes.
	void lastErrorChanged();

public slots:

	//! Reports an error.
	void reportError( const ErrorInfo& error );

// Private interface.
private:

	//! Refreshes the last error.
	void refreshLastError();

// Private data.
private:

	int m_unseenError;  //!< Index of the last error that the user has not acknowledged.
	ErrorModel* m_lastError;  //!< The error model for the last error.
	QVector< ErrorInfo > m_errors;  //!< A collection errors that have recently occurred.

};

#endif // APPMONITOR_H
