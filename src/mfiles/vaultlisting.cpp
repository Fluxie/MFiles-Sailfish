#include "vaultlisting.h"

#include "../errors/errorinfo.h"
#include "../mfwsrest.h"

VaultListing::VaultListing(QObject *parent) :
	QObject(parent)
{
}

void VaultListing::reportNetworkError( QNetworkReply::NetworkError code, const QString& description )
{
	Q_UNUSED( code ) // TODO.

	// Convert to our error object and emit.
	ErrorInfo errorinfo( description );
	PUSH_ERROR_LAYER( errorinfo );
	emit error( errorinfo );
}

MfwsRest* VaultListing::rest() const
{
	// Create the MfwsRest object for fetching the actual data.
	if( m_rest == nullptr )
	{
		m_rest = new MfwsRest( this->host(), const_cast< VaultListing* >( this ) );
		m_rest->setUsername( this->username() );
		m_rest->setPassword( this->password() );
		QObject::connect( m_rest, &MfwsRest::error, this, &CoreBase::reportNetworkError );
		QObject::connect( this, &VaultListing::usernameChanged, m_rest, &MfwsRest::setUsername );
		QObject::connect( this, &VaultListing::passwordChanged, , m_rest, &MfwsRest::setPassword );

	}  // end if.

	// Return the rest.
	return m_rest;
}
