#include "listingid.h"

const ListingId ListingId::Root = ListingId( "/views/items" );

ListingId::ListingId()
{
}

/**
 * @brief ListingId
 */
ListingId::ListingId( const QString& path ) :
	m_path( path )
{

}

bool ListingId::operator==( const ListingId& comparedTo ) const
{
	return m_path == comparedTo.m_path;
}
