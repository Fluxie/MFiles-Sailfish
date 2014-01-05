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

#include "errorinfo.h"

ErrorInfo::ErrorInfo( const QString& message ) :
	m_message( message )
{
}

//! Copy-constructor.
ErrorInfo::ErrorInfo( const ErrorInfo& copy ) :
	m_message( copy.message() ),
	m_layers( copy.m_layers )
{

}

//! Assignment.
void ErrorInfo::operator=( const ErrorInfo& assigned )
{
	m_message = assigned.message();
	m_layers = assigned.m_layers;
}

//! Adds new layer to the error.
void ErrorInfo::push_layer(
		const QString& file,
		int lineNumber,
		const QString& className
) const
{
	// Add the layer
	m_layers.push_back( ErrorLayer( file, lineNumber, className, "" ));
}
