#include "errorinfo.h"

ErrorInfo::ErrorInfo( const QString& message ) :
	m_message( message )
{
}

//! Copy-constructor.
ErrorInfo::ErrorInfo( const ErrorInfo& copy ) :
	m_message( copy.message() ),
	m_layers( copy.m_layers )
{

}

//! Assignment.
void ErrorInfo::operator=( const ErrorInfo& assigned )
{
	m_message = assigned.message();
	m_layers = assigned.m_layers;
}

//! Adds new layer to the error.
void ErrorInfo::push_layer(
		const QString& file,
		int lineNumber,
		const QString& className
) const
{
	// Add the layer
	m_layers.push_back( ErrorLayer( file, lineNumber, className, "" ));
}
