#ifndef ERRORINFO_H
#define ERRORINFO_H

#include <QMetaType>
#include <QString>

//! Represents a single error within the application.
class ErrorInfo
{
// Public interface.
public:

	//! Initializes empty error. Required for Qt metatype system.
	ErrorInfo() {}

	//! Initializes new error info.
	ErrorInfo( const QString& message );

	//! Copy-constructor.
	ErrorInfo( const ErrorInfo& copy );

	//! Destructor.
	~ErrorInfo() {}

	//! Assignment.
	void operator=( const ErrorInfo& assigned );

	//! Message.
	const QString& message() const { return m_message; }

// Private data.
private:

	QString m_message;  //!< A short error message.
};

//! Declare for Qt.
Q_DECLARE_METATYPE( ErrorInfo )

#endif // ERRORINFO_H
