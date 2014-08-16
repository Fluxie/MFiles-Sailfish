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

#include "../backend/corebase.h"
#include "../backend/vaultcore.h"

#include "frontbase.h"
#include "vaultfront.h"

FrontBase::FrontBase( QObject* core, QObject* parent ) :
	QObject( parent ),
	m_core( core )
{

}

bool FrontBase::isPartOf( const VaultFront* vault ) const
{
	Q_CHECK_PTR( vault );

	if( m_core == nullptr )
		return false;

	// Make the check.
	const VaultCore* vaultCore = qobject_cast< const VaultCore* >( vault->coreConst() );
	const CoreBase* ourCore = qobject_cast< const CoreBase* >( m_core );
	return ourCore->isPartOf( vaultCore );
}

void FrontBase::setCore( QObject* core )
{
	if( m_core == core )
		return;

	m_core = core;
	emit coreChanged();
}

//! A core of the given type has become available.
void FrontBase::coreAvailable( QObject* core, QObject* source )
{
	// Store the core if it is accepted to this object.
	qDebug( "Core available" );
	if( this->accept( core ) )
		this->setCore( core, source );
}

//! Sets the core.
void FrontBase::setCore( QObject* core, QObject* source )
{
	// Store the core, disconnect from the core source and notify listeners.	
	qDebug( "Set core" );
	m_core = core;
	source->disconnect( this, "coreAvailable" );
	emit coreChanged();
}
