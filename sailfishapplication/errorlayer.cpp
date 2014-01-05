#include "errorlayer.h"

ErrorLayer::ErrorLayer(
		const QString& file,
		int lineNumber,
		const QString& className,
		const QString& contextId
	) :
	m_file( file ),
	m_lineNumber( lineNumber ),
	m_className( className ),
	m_contextId( contextId )
{	
}

//! Copy-constructor.
ErrorLayer::ErrorLayer( const ErrorLayer& copy ) :
	m_file( copy.m_file ),
	m_lineNumber( copy.m_lineNumber ),
	m_className( copy.className() ),
	m_contextId( copy.m_contextId )
{
}
