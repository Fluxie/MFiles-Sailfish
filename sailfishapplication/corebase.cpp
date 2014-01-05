#include "corebase.h"

#include "mfwsrest.h"
#include "vaultcore.h"

#include <QThread>

CoreBase::CoreBase( VaultCore* owner ) :
	QObject( 0 ),
	m_vault( owner ),
	m_rest( 0 ),
	m_state( Initializing )
{
	Q_CHECK_PTR( m_vault );

	// Transfer to correct thread.
	this->moveToThread( owner->thread() );
}

//! Initializes new core base.
CoreBase::CoreBase( VaultCore* owner, QObject* parent ) :
	QObject( 0 ),
	m_vault( owner ),
	m_rest( 0 ),
	m_state( Initializing )
{
	Q_CHECK_PTR( m_vault );

	// Transfer to correct thread.
	if( parent != 0 && owner->thread() != parent->thread() )
		qCritical( "Owner and parent must live in the same thread." );
	this->moveToThread( owner->thread() );
	if( parent != 0 )
		this->setParent( parent );

	// Initialize the core.
	initializeBase( parent );
}

//! Initializes the core. Must be called from the thread that owns the core.
void CoreBase::initializeBase( QObject* parent )
{
	// Set the parent.
	if( parent != 0 && this->thread() != parent->thread() )
		qCritical( "Not in the thread of this object." );
	if( this->parent() != parent )
		this->setParent( parent );	

	// The core is now initialized.
	m_state = Initialized;
	emit initialized();
}

//! A network error has occurred within the core.
void CoreBase::reportNetworkError( QNetworkReply::NetworkError code, const QString& description )
{
	// Convert to our error object and emit.
	ErrorInfo errorinfo( description );
	emit error( errorinfo );
}

//! An error has occurred within the core or in an object related to this core.
void CoreBase::reportError( const ErrorInfo& errorinfo )
{
	// Re-emit
	emit error( errorinfo );
}

//! Accesses the MFWS REST API.
MfwsRest* CoreBase::rest()
{
	// Create the MfwsRest object for fetching the actual data.
	if( m_rest == 0 )
	{
		m_rest = new MfwsRest( this->vault()->url(), this );
		m_rest->setAuthentication( this->vault()->authentication() );
		QObject::connect( m_rest, &MfwsRest::error, this, &CoreBase::reportNetworkError );
		QObject::connect( this->vault(), &VaultCore::authenticationChanged, m_rest, &MfwsRest::setAuthentication );

	}  // end if.

	// Return the rest.
	return m_rest;
}
