#ifndef VALUELISTCACHE_H
#define VALUELISTCACHE_H

#include <QHash>
#include <QJsonArray>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QVector>

// Forward declarations.
class QNetworkReply;
class MfwsRest;
class ValueListCore;
class VaultCore;

class ValueListCache : public QObject
{
	Q_OBJECT
public:
	explicit ValueListCache(VaultCore* parent );

	//! Gets data of one specific value list.
	ValueListCore* get( int valueList );

	void setContentFrom( QNetworkReply* reply );

signals:

	//! Signaled when the available value lists have been refreshed.
	void refreshed();

public slots:

	//! Requests the cache refresh.
	void requestRefresh();

// Private data.
private:

	// Static variables that do not need protection
	VaultCore* m_owner;  //!< Owner of this cache.
	MfwsRest* m_rest;  //!< Access to M-Files REST API.

	QMutex m_mutex;  //!< Mutex protecting the shared
	bool m_populated;  //!< True when the cache has been populated.
	QJsonArray m_valueLists;  //!< Available value lists.
	QHash< int, ValueListCore* > m_cache;  // Cached value listss
};

#endif // VALUELISTCACHE_H
