#include "errorinfo.h"

ErrorInfo::ErrorInfo( const QString& message ) :
	m_message( message )
{
}

//! Copy-constructor.
ErrorInfo::ErrorInfo( const ErrorInfo& copy ) :
	m_message( copy.message() )
{

}

//! Assignment.
void ErrorInfo::operator=( const ErrorInfo& assigned )
{
	m_message = assigned.message();
}
