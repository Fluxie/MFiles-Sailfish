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

#ifndef HOSTCORE_H
#define HOSTCORE_H

#include <QJSValue>
#include <QThread>
#include <QVariant>

#include "../errors/errorinfo.h"

// Forward declarations.
class QQmlEngine;
class AppMonitor;
class VaultCore;

/**
 * @brief HostCore manages the background thread for background operations.
 */
class HostCore : public QThread
{
    Q_OBJECT
public:

	/**
	 * @brief Initializes new HostCore object.
	 * @param monitor Global application monitor.
	 */
	explicit HostCore( AppMonitor* monitor );

	/**
	 * @brief Gets the global HostCore instance.
	 * @return Global host core instance.
	 */
	static HostCore* instance();

	/**
	 * @brief Creates a new JS object to the global QML engine.
	 * @return New JavaScript object as QJSValue
	 */
	static QJSValue newJSObject();

	/**
	 * @brief Converts the given variant QJSValue that can be used in the JavaScript engine.
	 * @param Variant.
	 * @return Variant as QJSValue
	 */
	static QJSValue toJSValue( const QVariant& var );

	/**
	 * @brief Converts the given QObject QJSValue that can be used in the JavaScript engine.
	 * @param QObject.
	 * @param javaScriptOwnership True to set the ownership of the object to JavaScript.
	 * @return QObject as QJSValue
	 */
	static QJSValue toJSValue( QObject* object );

	/**
	 * @brief Global QML engine running the UI code.
	 * @return The global QML engine.
	 */
	QQmlEngine* qmlEngine() const { return m_engine; }

	/**
	 * @brief Sets
	 * @param engine
	 */
	void setQmlEngine( QQmlEngine* engine ) { m_engine = engine; }


    
signals:

	/**
	 * @brief This signal is emitted when an error occurss within the application.
	 * @param error Description of the error.
	 */
	void error( const ErrorInfo& error );
    
public slots:

	/**
	 * @brief Prepares new VaultCore object for accessing the specified vault.
	 * @param url http url to the vaul.
	 * @param authentication Authentication token for accessing the vault's resources.
	 * @param name The name of the new vault.
	 * @return VautlCore
	 */
	VaultCore* prepareVault(
		const QString& url,
		const QString& authentication,
		const QString& name
	);

	/**
	 * @brief Reports an error somewhere within the application.
	 * @param error Description of the error.
	 */
	void reportError( const ErrorInfo& error );

// Private interface.
private:

	/**
	 * @brief Event loop of the background thrad.
	 */
	virtual void run();

// Private data.
private:

	AppMonitor* m_monitor;  //!< Global application monitor.
	QQmlEngine* m_engine;  //!< Global JavaScript engine.
};

#endif // HOSTCORE_H
