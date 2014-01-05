#ifndef COREBASE_H
#define COREBASE_H

#include <QObject>
#include <QNetworkReply>

#include "errorinfo.h"

// Forward declarations.
class MfwsRest;
class VaultCore;

class CoreBase : public QObject
{
	Q_OBJECT
public:

	//! State of the core.
	enum State
	{
		Initializing,  //! The core is still being initialized.
		Initialized  //!< The core has been initialized.
	};

	//! Initializes new core base.
	explicit CoreBase( VaultCore* owner );

	//! Initializes new core base.
	explicit CoreBase( VaultCore* owner, QObject* parent );

	//! Is this core initializaed.
	bool isInitialized() { return m_state == Initialized; }

signals:

	//! Signaled when the core becomes initialized.
	void initialized();

	//! Signaled when an error occurs in the procssing of a request to the core.
	void error( const ErrorInfo& error );

public slots:

	//! Initializes the core. Must be called from the thread that owns the core.
	void initializeBase( QObject* parent );

	//! A network error has occurred within the core.
	void reportNetworkError( QNetworkReply::NetworkError code, const QString& description );

// Protected interface.
protected:

	//! Accesses the vault.
	VaultCore* vault() { return m_vault; }

	//! Accesses the MFWS REST API.
	MfwsRest* rest();

// Private data.
private:

	VaultCore* m_vault;  //! Vault.
	MfwsRest* m_rest;  //!< Access to M-Files REST API.
	State m_state;  //!< The state of the core.

};

#endif // COREBASE_H
