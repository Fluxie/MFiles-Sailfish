#include "appmonitor.h"

AppMonitor::AppMonitor(QObject *parent) :
	QObject(parent),
	m_unseenError( 0 )
{
}

//! The last error messagge.
QString AppMonitor::lastError()
{
	// Errors that the user has not seen?
	if( m_errors.size() == m_unseenError )
		return "";

	// Report the latest error.
	return m_errors.last().message();
}

//! Hides all current errors from the user.
void AppMonitor::hideErrors()
{
	// Clear the errors.
	int previouslySeenError = m_unseenError;
	m_unseenError = m_errors.size();
	if( previouslySeenError != m_unseenError )
		emit lastErrorChanged();
}

//! Reports an error.
void AppMonitor::reportError( const ErrorInfo& error )
{
	// Store the error.
	qDebug( error.message().toLatin1() );
	m_errors.push_back( error );
	emit lastErrorChanged();
}
