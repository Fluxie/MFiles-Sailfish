/*
 *  Copyright 2013 Juha Lepola
 *
 *  This file is part of M-Files for Sailfish.
 *
 *  M-Files for Sailfish is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  M-Files for Sailfish is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with M-Files for Sailfish. If not, see
 *  <http://www.gnu.org/licenses/>.
 */

#ifndef COREBASE_H
#define COREBASE_H

#include <QObject>
#include <QNetworkReply>

#include "../errors/errorinfo.h"

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

	//! An error has occurred within the core or in an object related to this core.
	void reportError( const ErrorInfo& errorinfo );

// Protected interface.
protected:

	//! Accesses the vault.
	VaultCore* vault() { return m_vault; }

	//! Accesses the vault.
	const VaultCore* vault() const { return m_vault; }

	//! Accesses the MFWS REST API.
	MfwsRest* rest() const;

// Private data.
private:

	VaultCore* m_vault;  //! Vault.
	mutable MfwsRest* m_rest;  //!< Access to M-Files REST API.
	State m_state;  //!< The state of the core.

};

#endif // COREBASE_H
