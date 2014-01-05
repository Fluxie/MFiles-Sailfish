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
	qDebug( error.message().toLatin1() );
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
