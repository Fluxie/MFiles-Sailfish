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
 * CoreBase provides common functionality for objects that live in the background thread.
 * When created the CoreBase automatically moves itself to the VaultCore's thread.
 * The parent of the CoreBase is set indirectly to allow creating it in the UI thread.
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

	/**
	 * @brief This signal is emitted when an error occurs in the procssing of a request to the core.
	 * @param error Description of the error.
	 */
	void error( const ErrorInfo& error );

public slots:

	/**
	 * @brief Reports an error that occurred within the core or in an object related to this core.
	 * @param errorinfo
	 */
	void reportError( const ErrorInfo& errorinfo );

// Protected interface.
protected:

	/**
	 * @brief Accesses the vault object of the core.
	 * @return Vault of this core.
	 */
	VaultCore* vault() { return m_vault; }

	/**
	 * @brief Accesses the vault object of the core.
	 * @return Vault of this core.
	 */
	const VaultCore* vault() const { return m_vault; }

	/**
	 * @brief Accesses M-Files REST API client for making REST requests.
	 * @return M-Files REST API client
	 */
	MfwsRest* rest() const;

private slots:

	/**
	 * @brief Reports a network error that occurred within the core.
	 * @param code Error code.
	 * @param description Description of the error.
	 */
	void reportNetworkError( QNetworkReply::NetworkError code, const QString& description );

	/**
	 * @brief Updates the parent of this object.
	 * @param parent New parent.
	 */
	void updateParent( QObject* parent ) { this->setParent( parent ); }

// Private data.
private:

	VaultCore* m_vault;  //! Vault.
	mutable MfwsRest* m_rest;  //!< Access to M-Files REST API.	

};

#endif // COREBASE_H
