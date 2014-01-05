#include "errormodel.h"

#include <QQmlEngine>
#include <QVector>

#include "errorlayer.h"

ErrorModel::ErrorModel(QObject *parent) :
	QObject(parent)
{
}

//! Constructrs an error model based on the given error.
ErrorModel::ErrorModel( const ErrorInfo& error ) :
	m_error( error )
{
	// Fill the stack string list.
	fillStack();
}

//! Gets the error stack.
QStringListModel* ErrorModel::stack() const
{
	// Empty stack?
	if( m_stack.empty() )
		return 0;

	// Create and return the model
	QStringListModel* model = new QStringListModel( m_stack );
	QQmlEngine::setObjectOwnership( model, QQmlEngine::JavaScriptOwnership );
	return model;
}

//! Fills the stack using the current error information.
void ErrorModel::fillStack()
{
	// Clear previous values.
	m_stack.clear();

	// Convert the error layers to textual error stack.
	const QVector< ErrorLayer>& layers = m_error.layers();
	for( QVector< ErrorLayer>::const_iterator itr = layers.constBegin(); itr != layers.constEnd(); itr++ )
	{
		// Cosntruct description for the layer.
		// We remove all possiple path elements from the file name in order make the description shorter.
		const ErrorLayer& layer = *itr;
		QString fileName = layer.file().section( '/', -1, -1 );
		QString asString = QString( "%1, %2, %3" ).arg( layer.className() ).arg( fileName ).arg( layer.lineNumber() );
		m_stack.append( asString );

	}  // end for.
}
