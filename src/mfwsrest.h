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

#ifndef MFWSREQUEST_H
#define MFWSREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonValue>

// Forward declaration.
class QJsonDocument;

//! Helper class for making MFWS REST API calls.
class MfwsRest : public QObject
{
	Q_OBJECT
public:

	//! Constructor.
	explicit MfwsRest(
		const QString& url,
		QObject *parent
	);

	//! Destructor.
	virtual ~MfwsRest();
	
	//! Gets resources.
	QNetworkReply* getJson(
		const QString& resource  //!< The name of the resource.
	) const;

	//! Posts resources.
	QNetworkReply* postJson(
		const QString& resource,  //!< The name of the resource.
		const QJsonDocument& post  //!< The object to post.
	) const;


	//! Puts resource.
	QNetworkReply* putJson(
		const QString& resource,  //!< The name of the resource.
		const QJsonDocument& put  //!< The value to post.
	) const;

signals:

	//! Signaled when error occurs in the prosessing of the request.
	void error( QNetworkReply::NetworkError code, const QString& description ) const;
	
public slots:

	/**
	 * @brief Sets the username..
	 * @param New username.
	 */
	void setUsername( const QString& username ) { m_username = username; }

	/**
	 * @brief Sets the password-
	 * @param New password.
	 */
	void setPassword( const QString& password ) { m_password = password; }
		
	/**
	 * @brief Sets the authentication information.
	 * @param authentication
	 */
	void setAuthentication( const QString& authentication ) { m_authentication = authentication; }

// Private interface.
private:

	//! Prepares request.
	QNetworkRequest prepareRequest( const QString& resource ) const;

	/**
	 * @brief Fills authentication information to the given request.
	 * @param The request to set the authentication information.
	 * @return True if the authentication information was available.
	 */
	bool fillAuthentication( QNetworkRequest& request ) const;

private slots:

	/**
	 * @brief Updates the parent of this object.
	 * @param parent New parent.
	 */
	void updateParent( QObject* parent ) { this->setParent( parent ); }

// Private data.
private:

	QString m_url;
	QString m_username;
	QString m_password;
	QString m_authentication;
	QNetworkAccessManager* m_network;
};

#endif // MFWSREQUEST_H
