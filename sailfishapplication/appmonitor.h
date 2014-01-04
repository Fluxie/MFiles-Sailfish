#ifndef APPMONITOR_H
#define APPMONITOR_H

#include <QObject>
#include <QVector>

#include "errorinfo.h"

//! Application monitor for monitoring the status of the application for errors and other related information.
class AppMonitor : public QObject
{
	Q_OBJECT
public:

	//! Initializes the AppMonitor.
	explicit AppMonitor( QObject *parent = 0 );

signals:

public slots:

	//! Reports an error.
	void reportError( const ErrorInfo& error );

// Private data.
private:

	QVector< ErrorInfo > m_errors;  //!< A collection errors that have recently occurred.

};

#endif // APPMONITOR_H
