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

#include "asyncfetch.h"


AsyncFetch::AsyncFetch( int cookie ) :
	QObject( 0 ),
	m_state( AsyncFetch::Fetching ),
	m_cookie( cookie ),
	m_filteringUpToDate( false )
{
}

/**
 * @brief value
 * @return Retrieves the value.
 */
QJsonValue AsyncFetch::value() const
{
	// Filter if not done.
	if( ! m_filteringUpToDate )
	{
		m_filteredValues = this->applyFilter( m_values );
		m_filteringUpToDate = true;
	}

	// Return the value.
	Q_ASSERT( m_values.size() <= 1 );
	if( m_values.size() == 1 )
	{
		// Return the first and only value.
		return *m_values.constBegin();
	}
	else
	{
		// No value.
		return QJsonValue::Null;
	}
}

QJsonArray AsyncFetch::values() const
{
	// Filter if not done.
	if( ! m_filteringUpToDate )
	{
		m_filteredValues = this->applyFilter( m_values );
		m_filteringUpToDate = true;
	}

	return m_filteredValues;
}

void AsyncFetch::itemFetched( int cookie, const QJsonValue& value )
{
	// We are reciving notifications for all fetched items.
	// Only notify if the item was the one we were waiting for.
	if( m_cookie == cookie && m_state == AsyncFetch::Fetching )
	{
		// Store the value.
		m_values.append( value );
		m_filteringUpToDate = false;

		// Fetch completed.
		m_state = AsyncFetch::Finished;
		emit finished();
	}
	else
	{
		// With the current implementation this should never happen.
		Q_ASSERT( false );

	} // end if
}

void AsyncFetch::itemsFetched( int cookie, const QJsonArray& values )
{
	// We are reciving notifications for all fetched items.
	// Only notify if the item was the one we were waiting for.
	if( m_cookie == cookie && m_state == AsyncFetch::Fetching )
	{
		// Store the value.
		m_values = values;
		m_filteringUpToDate = false;

		// Fetch completed.
		m_state = AsyncFetch::Finished;
		emit finished();
	}
	else
	{
		// With the current implementation this should never happen.

	} // end if
}

void AsyncFetch::reportError( int cookie )
{
	// We are reciving notifications for all fetched items.
	// Only notify if the item was the one we were waiting for.
	if( m_cookie == cookie && m_state == AsyncFetch::Fetching )
	{
		// Error.

		// Fetch completed.
		m_state = AsyncFetch::Error;
		emit error();

	}
	else
	{
		// With the current implementation this should never happen.
		Q_ASSERT( false );

	} // end if
}

/**
 * @brief This virtual function is called when something has been connected to signal in this object.
 * @param signal
 */
void AsyncFetch::connectNotify ( const QMetaMethod& signal )
{
	// Signal the finished or error signal if we were aleady signaled.
	if( signal.name() == QByteArray( "finished" ) && m_state == AsyncFetch::Finished )
	{
		// The fetch operation had already completed.
		// Notify the object that just connected to us.
		emit finished();
	}
	else if( signal.name() == QByteArray( "error" ) && m_state == AsyncFetch::Error )
	{
		// The fetch operation had already completed.
		// Notify the object that just connected to us.
		emit error();
	}
	else
	{

	}
}

QJsonArray AsyncFetch::applyFilter( const QJsonArray& values ) const
{
	// No need to filter?
	if( m_filters.empty() )
		return values;

	// Filter the values if there are filters present.
	QJsonArray filtered;
	foreach( const QJsonValue& value, values )
	{
		// Check if the value is accepted.
		bool accepted = true;
		foreach( FILTER_T filter, m_filters )
		{
			if( ! filter( value ) )
				accepted = false;
		}

		// Include the value.
		if( accepted )
			filtered.append( value );

	}  // end foreach.
	return filtered;
}
