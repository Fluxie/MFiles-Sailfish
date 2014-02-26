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

#ifndef VALUELISTKEY_H
#define VALUELISTKEY_H

#include "typedvaluefilter.h"

/**
 * @brief The ValueListKey class specified a lookup key for cached value lists.
 */
class ValueListKey
{
public:

	/**
	 * @brief Initializes new ValueListKey object.
	 * @param idIn The id of the value list.
	 * @param filterIn  //!< The filter used to filter the value list.
	 */
	ValueListKey( int idIn, const TypedValueFilter* filterIn ) :
		id( idIn ),
		filter( filterIn ? new TypedValueFilter( *filterIn ) : 0 )
	{}

	//! Destructor.
	~ValueListKey() { if( filter ) filter->deleteLater(); }

	/**
	 * @brief Copy-contstructor.
	 * @param key Original.
	 */
	explicit ValueListKey( const ValueListKey& key ) :
		id( key.id ),
		filter( key.filter ? new TypedValueFilter( *key.filter ) : 0 )
	{}

	int id;
	TypedValueFilter* filter;
};

/**
 * @brief Comparison operator for ValueListKey objects.
 * @param left  Left operand.
 * @param right Right operand.
 * @return Returns true if the compared objects are equal.
 */
inline bool operator==( const ValueListKey &left, const ValueListKey &right )
{	
	// Check if one of the keys is missing the filter while the other is not.
	if( ( left.filter == 0 && right.filter != 0 ) ||
		( left.filter != 0 && right.filter == 0 ) )
		return false;

	// Make the equality comparison.
	return left.id == right.id
		   && ( left.filter == right.filter ||  // This will be true if both are NULL
				*left.filter == *right.filter );  // The possibility of other being NULL was checked earlier.
}

/**
 * @brief Hash function for ValueListKey object.
 * @param key The ValueListKey object for which the hash value is calculated.
 * @return Hash value of the ValueListKey object.
 */
inline uint qHash(const ValueListKey &key)
{
	return qHash( key.id ) ^ qHash( key.filter );
}


#endif // VALUELISTKEY_H
