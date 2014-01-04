#ifndef APPMONITOR_H
#define APPMONITOR_H

#include <QObject>
#include <QVector>

#include "errorinfo.h"

//! Application monitor for monitoring the status of the application for errors and other related information.
class AppMonitor : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )
public:

	//! Initializes the AppMonitor.
	explicit AppMonitor( QObject *parent = 0 );

	//! The last error messagge.
	QString lastError();

	//! Hides all current errors from the user.
	Q_INVOKABLE void hideErrors();

signals:

	//! Signaled when the last error changes.
	void lastErrorChanged();

public slots:

	//! Reports an error.
	void reportError( const ErrorInfo& error );

// Private data.
private:

	int m_unseenError;  //!< Index of the last error that the user has not acknowledged.
	QVector< ErrorInfo > m_errors;  //!< A collection errors that have recently occurred.

};

#endif // APPMONITOR_H
