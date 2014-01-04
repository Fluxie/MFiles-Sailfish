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

#include "mfwsrest.h"

#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QEventLoop>

MfwsRest::MfwsRest( const QString& url, QObject *parent ) :
	QObject(parent),
	m_url( url )
{
	m_network = new QNetworkAccessManager( this );

	qDebug( "MfwsRest instantiated." );
}

//! Destructor.
MfwsRest::~MfwsRest()
{
	qDebug( "MfwsRest destroyed." );
}

//! Gets resources
QNetworkReply* MfwsRest::getJson(
	QString& resource  //!< The name of the resource.
) const
{
// 	QSslConfiguration config(QSslConfiguration::defaultConfiguration());



	// SSL

	// request.setSslConfiguration(config);



	// Prepare the request.
	QNetworkRequest request;
	if( m_authentication.size() == 0 )
		qCritical( "Authentication information is not available." );
	request.setRawHeader( "X-Authentication", m_authentication.toLatin1() );
	request.setRawHeader("Accept", "application/json");
	request.setRawHeader("Content-Type", "application/json");
	QString finalUrl = QString("%1/REST%2").arg( m_url, resource );
	request.setUrl( QUrl( finalUrl ) );

	qDebug() << finalUrl;

	// Execute the request and read reply.
	QNetworkReply* reply = m_network->get( request );
	QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		reply->deleteLater(); } );
	return reply;
}
