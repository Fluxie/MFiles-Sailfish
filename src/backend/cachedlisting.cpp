#include "cachedlisting.h"

#include <QMutexLocker>
#include <QNetworkReply>

#include "asyncfetch.h"
#include "listingcache.h"

CachedListing::CachedListing( ListingCache* parent, const ListingId& id ) :
	ListResourceCacheBase( id.path(), parent->vault() ),
	m_id( id ),
	m_owner( parent )
{

	qDebug( "Cached listing instantiated." );
}

