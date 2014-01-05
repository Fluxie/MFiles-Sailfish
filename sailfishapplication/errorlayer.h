#ifndef ERRORLAYER_H
#define ERRORLAYER_H

#include <QString>

//! Error layer act
class ErrorLayer
{
public:

	//! Default constructor, required for vector.
	ErrorLayer() {}

	//! Initializes new layer.
	ErrorLayer(
			const QString& file,
			int lineNumber,
			const QString& className,
			const QString& contextId
	);

	//! Copy-constructor.
	ErrorLayer( const ErrorLayer& copy );

	//! File where this layer was created.
	const QString& file() const { return m_file; }

	//! The line number within the file where this layer was created.
	int lineNumber() const { return m_lineNumber; }

	//! The class name where the layer was added.
	const QString& className() const { return m_className; }

	//! The context where this layer was created.
	const QString& contextId() const { return m_contextId; }

private:

	QString m_file;  //!< The file where this layer was added.
	int m_lineNumber;  //!< The line number.
	QString m_className;  //!< The name of the class where the layer was added.
	QString m_contextId;  //!< Identifies the context. This information can be used to filter the errors that will be shown to the user.
};

#endif // ERRORLAYER_H
