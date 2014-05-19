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

#include "frontbase.h"

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
	emit refreshed();
}
