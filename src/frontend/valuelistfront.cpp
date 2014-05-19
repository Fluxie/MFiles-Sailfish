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

#include "valuelistfront.h"


#include "../backend/objecttypecache.h"
#include "../backend/valuelistcore.h"
#include "../backend/vaultcore.h"

ValueListFront::ValueListFront(
	VaultCore* vault,  //!< Vault.
	ValueListCore* core  //!< Core
) :
	FrontBase( core ),
	m_id( core->id() ),
	m_filter( 0 ),
	m_vault( vault )
{
	// Create filter.
	if( core->filter() )
		m_filter = new TypedValueFilter( *core->filter(), this );

	// Connect refresh events.
	QObject::connect( core, &ValueListCore::refreshed, this, &ValueListFront::refreshed );

	// Connect status changed events:
	// - Core has been populated.
	QObject::connect( core, &ValueListCore::populatedChanged, this, &ValueListFront::statusChanged );
}

//! Value list items.
AsyncFetch* ValueListFront::items()
{
	// Empty?
	if( ! this->core() )
	{
		qCritical( "TODO: Report Error." );
		return 0;
	}

	// Fetch the core and return the value list.
	ValueListCore* core = this->valueList();
	return core->list();
}

//! Fetches the specified value list item.
AsyncFetch* ValueListFront::item( int id )
{
	// Empty?
	if( ! this->core() )
	{
		qCritical( "TODO: Report Error." );
		return 0;
	}

	// Fetch the core and return the fetch operation.
	ValueListCore* core = this->valueList();
	return core->get( id );
}

//! Status.
ValueListFront::Status ValueListFront::status()
{
	// Disconnected?
	if( this->core() == 0 )
		return ValueListFront::Disconnected;

	// Is the value list empty.
	ValueListCore* core = this->valueList();
	if( ! core->populated() )
		return ValueListFront::Empty;

	// We are in a ready state.
	return ValueListFront::Ready;
}

//! Does this front accept the specified core.
bool ValueListFront::accept( QObject* coreCandidate ) const
{
	// Can we accept the specified core.
	qDebug( "Requesting acceptance for ValueListCore." );
	ValueListCore* core = qobject_cast< ValueListCore* >( coreCandidate );
	if( core != 0 && core->id() == m_id && core->filter() == m_filter )
		return true;
	else
		return false;
}
