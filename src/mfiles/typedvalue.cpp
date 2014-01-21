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

#include "typedvalue.h"

#include <QJsonArray>

TypedValue::TypedValue( QJsonValue typedValue ) :
	MFilesTypeCapsule( typedValue )
{
}

/**
 * @brief Initializes a new lookup based typed value
 */
TypedValue::TypedValue( int dataType, const Lookup& lookup )
{
	qDebug( "TypedValue" );
	Q_ASSERT( dataType == 9 || dataType == 10 );

	// Get reference to the internal typed value variable and set the values.
	QJsonObject& typedValue = this->object();
	typedValue[ "DataType" ] = dataType;
	typedValue[ "HasValue" ] = ! lookup.isUndefined();
	typedValue[ "DisplayValue" ] = lookup.displayValue();

	// Set the value based on the data type.
	switch ( dataType )
	{
	// Single-select.
	case 9:
	{
		typedValue[ "Lookup" ] = lookup.value();
		typedValue[ "Value" ] = lookup.value();
		break;
	}

	// Multi-select
	case 10:
	{
		QJsonArray lookups;
		lookups.push_back( lookup.value() );
		typedValue[ "Lookups" ] = lookups;
		typedValue[ "Value" ] = lookups;
		break;
	}

	// Unexpected data type.
	default:
		qCritical( "TODO: Report error." );
		break;
	}
}

TypedValue::TypedValue( const QJsonArray& lookups )
{
	// Get reference to the internal typed value variable and set the values.
	QJsonObject& typedValue = this->object();
	typedValue[ "DataType" ] = 10;
	typedValue[ "HasValue" ] = lookups.size() != 0;
	typedValue[ "DisplayValue" ] = QString( "Demo" );
	typedValue[ "Lookups" ] = lookups;
	typedValue[ "Value" ] = lookups;
	qDebug( QString( "TypedValue, hasValue %1, DisplayValue %2, Lookup count %3" ).arg( this->hasValue() ).arg( "TODO" ).arg( lookups.count() ).toLatin1() );
	Q_ASSERT( this->hasValue() || lookups.count() == 0 );
	Q_ASSERT( this->asLookups().count() == lookups.count() );
}

/**
 * @brief asLookups
 * @return All lookups as a Json array. Can be called for both single-select and multi-select lookups.
 */
QJsonArray TypedValue::asLookups() const
{
	QJsonArray lookups;
	if( ! this->hasValue() )
		return lookups;
	switch ( this->dataType() )
	{
	// Single-select.
	case 9:
	{
		qDebug( "Single lookup" );
		lookups.push_back( this->object()[ "Lookup" ] );
		break;
	}

	// Multi-select
	case 10:
	{
		qDebug( "Multi lookups" );
		foreach( Lookup lookup, this->object()[ "Lookups" ].toArray() )
		{
			qDebug( "Multi lookup" );
			lookups.push_back( lookup.value() );
		}
		break;
	}

	// Unexpected data type.
	default:
		qCritical( "TODO: Report error." );
		break;
	}

	return lookups;
}

/**
 * @brief getLookupIds
 * @return A collection of lookup ids.
 */
QSet< int > TypedValue::getLookupIds()
{
	Q_ASSERT( this->dataType() == 9 || this->dataType() == 10 );

	// Collect the ids of the lookups.
	QSet< int > lookupIds;
	foreach( Lookup lookup, this->asLookups() )
	{
		lookupIds.insert( lookup.item() );
	}
	return lookupIds;
}
