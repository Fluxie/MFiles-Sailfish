#include "appmonitor.h"

AppMonitor::AppMonitor(QObject *parent) :
	QObject(parent)
{
}

//! Reports an error.
void AppMonitor::reportError( const ErrorInfo& error )
{
	// Store the error.
	m_errors.push_back( error );
}
