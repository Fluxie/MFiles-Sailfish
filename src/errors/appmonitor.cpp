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

#include <QDebug>

#include "appmonitor.h"

#include "errormodel.h"

AppMonitor::AppMonitor(QObject *parent) :
	QObject(parent),
	m_unseenError( 0 ),
	m_lastError( 0 )
{
	// Set the last error.
	refreshLastError();
}

//! Destructor.
AppMonitor::~AppMonitor()
{
	if( m_lastError != 0 )
		m_lastError->deleteLater();
	m_lastError = 0;

}

//! Hides all current errors from the user.
void AppMonitor::hideErrors()
{
	// Clear the errors.
	int previouslySeenError = m_unseenError;
	m_unseenError = m_errors.size();
	if( previouslySeenError != m_unseenError )
		refreshLastError();
}

//! Reports an error.
void AppMonitor::reportError( const ErrorInfo& error )
{
	// Store the error.
	qDebug() << error.message();
	m_errors.push_back( error );
	refreshLastError();
}

//! Refreshes the last error.
void AppMonitor::refreshLastError()
{
	// Mark the previous error for deletion.
	if( m_lastError != 0 )
		m_lastError->deleteLater();
	m_lastError = 0;

	// Construct a new last error.
	if( m_unseenError != m_errors.size() )
	{
		// The last error that we should show to the user.
		m_lastError = new ErrorModel( m_errors[ m_unseenError ]);
	}
	else
	{
		// No error.
		m_lastError = new ErrorModel();
	}

	// The last error object was changed.
	emit lastErrorChanged();
}
