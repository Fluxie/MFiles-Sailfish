#ifndef LISTINGID_H
#define LISTINGID_H

#include <QHash>  // qHash functions for QString are defined in QHash
#include <QString>

// Forward declarations.
class CachedListing;

/**
 * @brief The ListingId identifies single listing object within the listing cache.
 */
class ListingId
{
public:

	/**
	 * @brief Id that identifies the root path.
	 */
	static const ListingId Root;

public:

	/**
	 * @brief ListingId
	 */
	ListingId();

	/**
	 * @brief ListingId
	 */
	explicit ListingId( const QString& path );

	/**
	 * @brief operator ==
	 * @param comparedTo The object this object is compared to.
	 * @returns True if the objects are equal.
	 */
	bool operator==( const ListingId& comparedTo ) const;

private:

	QString path() const { return m_path; }

private:

	// Allow access to hash functions
	friend unsigned int qHash( const ListingId &key, unsigned int seed );
	friend unsigned int qHash( const ListingId &key );

	friend class CachedListing;

	QString m_path;  //!< Path to the listing.
};

/**
 * @brief Hash function for ObjID object.
 * @param key The ObjID object for which the hash value is calculated.
 * @param seed Seed for the hash function.
 * @return Hash value of the ObjID object.
 */
inline unsigned int qHash( const ListingId &key, unsigned int seed )
{
	return seed ^ qHash( key.m_path );
}

/**
 * @brief Hash function for ObjID object.
 * @param key The ObjID object for which the hash value is calculated.
 * @return Hash value of the ObjID object.
 */
inline unsigned int qHash( const ListingId &key )
{
	return qHash( key.m_path );
}

#endif // LISTINGID_H
