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

#include "errorlayer.h"

ErrorLayer::ErrorLayer(
		const QString& file,
		int lineNumber,
		const QString& className,
		const QString& contextId
	) :
	m_file( file ),
	m_lineNumber( lineNumber ),
	m_className( className ),
	m_contextId( contextId )
{	
}

//! Copy-constructor.
ErrorLayer::ErrorLayer( const ErrorLayer& copy ) :
	m_file( copy.m_file ),
	m_lineNumber( copy.m_lineNumber ),
	m_className( copy.className() ),
	m_contextId( copy.m_contextId )
{
}
