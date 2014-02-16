/*
 *  Copyright 2014 Juha Lepola
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

#include "lazyownerinfo.h"


LazyOwnerInfo::LazyOwnerInfo( RESOLVER_T ownerInfoResolver  ) :
	QObject( 0 ),
	m_ownerInfoResolver( ownerInfoResolver ),
	m_cached( false )
{
}

/**
 * @brief Constructs a copy of LazyOwnerInfo object.
 * @param source The source of the copy.
 */
LazyOwnerInfo::LazyOwnerInfo( const LazyOwnerInfo& source, QObject* parent ) :
	QObject( parent ),
	m_ownerInfoResolver( source.m_ownerInfoResolver ),
	m_cached( source.m_cached ),
	m_cachedOwnership( source.m_cachedOwnership )
{
}

/**
 * @brief owner
 * @return The owner info.
 */
QJsonValue LazyOwnerInfo::ownerInfo() const
{
	// Resolve the owner information if it is not cached already.
	if( ! m_cached )
	{
		// Not cached, ask from resolver.
		m_cachedOwnership = m_ownerInfoResolver();
		m_cached = ! m_cachedOwnership.isNull() && ! m_cachedOwnership.isUndefined();
	}
	else if( ! m_cached )
	{
		// Return null as the subitem index is no longer valid.
		// Our state is basically invalid at this point and we cannot fetch the owner information
		return QJsonValue::Null;
	}
	else
	{
		// We have cached the ownership info already.

	}  // end if

	// Return the ownership information.	
	return m_cachedOwnership;
}
