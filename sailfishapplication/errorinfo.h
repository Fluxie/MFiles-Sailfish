#ifndef ERRORINFO_H
#define ERRORINFO_H

#include <QMetaType>
#include <QString>
#include <QVector>

#include "errorlayer.h"

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

	//! Adds new layer to the error.
	void push_layer(
			const QString& file,
			int lineNumber,
			const QString& contextId
	) const;

	//! Error layers.
	const QVector< ErrorLayer >& layers() const { return m_layers; }

// Private data.
private:

	QString m_message;  //!< A short error message.

	mutable QVector< ErrorLayer > m_layers;  //!< Error layers. Gathers the path the error was routed.
};

//! Declare for Qt.
Q_DECLARE_METATYPE( ErrorInfo )

// Define macro for pushing new error layer.
#define PUSH_ERROR_LAYER( _errorInfo ) \
{ \
	_errorInfo.push_layer( __FILE__, __LINE__, this->metaObject()->className() ); \
}

#endif // ERRORINFO_H
