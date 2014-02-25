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

/**
 * @brief Base object for "core" classes.
 *
 * Core classes are classes that execute operations in the background thread that must not block UI.
 */
class CoreBase : public QObject
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new core base without parent.
	 * @param owner Vault that owns this object.
	 *
	 */
	explicit CoreBase( VaultCore* owner );

	/**
	 * @brief Initializes new core base.
	 * @param owner Vault that owns this object.
	 * @param parent Parent for the core.
	 */
	explicit CoreBase( VaultCore* owner, QObject* parent );

signals:

	//! Signaled when an error occurs in the procssing of a request to the core.
	void error( const ErrorInfo& error );

public slots:

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

};

#endif // COREBASE_H
