#ifndef ERRORMODEL_H
#define ERRORMODEL_H

#include <QObject>
#include <QStringListModel>

#include "errorinfo.h"

class ErrorModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QString message READ message NOTIFY messageChanged )
	Q_PROPERTY( QStringListModel* stack READ stack NOTIFY stackChanged )
public:

	//! Constructrs empty error model.
	explicit ErrorModel(QObject *parent = 0);

	//! Constructrs an error model based on the given error.
	explicit ErrorModel( const ErrorInfo& error );

	//! Gets the error message.
	QString message() const { return m_error.message(); }

	//! Gets the error stack.
	QStringListModel* stack() const;

signals:

	//! Signaled when the message changes.
	void messageChanged();

	//! Signaled when the stack changes.
	void stackChanged();

public slots:

// Private interface.
private:

	//! Fills the stack based on the current error object.
	void fillStack();

// Private data
private:

	ErrorInfo m_error;
	QStringList m_stack;

};

#endif // ERRORMODEL_H
