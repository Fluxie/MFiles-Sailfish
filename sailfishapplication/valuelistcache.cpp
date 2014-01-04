#include "valuelistcache.h"

#include <QJsonDocument>
#include <QNetworkReply>

#include "mfwsrest.h"
#include "vaultcore.h"

ValueListCache::ValueListCache( VaultCore* parent ) :
	QObject( parent ),
	m_owner( parent ),
	m_populated( false )
{
	// Create the MfwsRest object for fetching the actual data.
	m_rest = new MfwsRest( m_owner->url(), this );
	m_rest->setAuthentication( m_owner->authentication() );
	QObject::connect( m_owner, &VaultCore::authenticationChanged, m_rest, &MfwsRest::setAuthentication );
}

//! Populates the cache.
void ValueListCache::requestRefresh()
{
	// Request the refresh.
//	QNetworkReply* reply = m_rest->getJson( "m_resource""" );
	//QObject::connect( reply, &QNetworkReply::finished,  [=]() {
		//this->setContentFrom( reply ); } );
}

//! Sets the cache content from the given network reply.
void ValueListCache::setContentFrom( QNetworkReply* reply )
{
	// Parse the returned JSON.
	QByteArray replyContent = reply->readAll();
	QJsonDocument result = QJsonDocument::fromJson( replyContent );
	QJsonArray results = result.array();

	// Populate the cache.
	{
		QMutexLocker lock( &m_mutex );

		// Clear previous data.
		m_cache.clear();
		m_valueLists = results;

		// Store the JSON objects that represents classes.
/*		for( QJsonArray::iterator itr = results.begin(); itr != results.end(); itr++ )
		{
			// Store the class.
			QJsonObject item = (*itr).toObject();
			int id = item[ "ID" ].toDouble();
			m_cache.insert( id, item );

		}  // end for*/
		m_populated = true;

	}  // end lock

	// The list has been refreshed.
	emit refreshed();
}
