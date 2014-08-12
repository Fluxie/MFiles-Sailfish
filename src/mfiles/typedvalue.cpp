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

#include "mfilesconstants.h"

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QUrl>

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

TypedValue::TypedValue( const QJsonValue& typedValue ) :
	MFilesTypeWrapper( typedValue )
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
	case MFiles::Constants::SingleSelectLookup :
	{
		typedValue[ "Lookup" ] = lookup.toJsonValue();
		typedValue[ "Value" ] = lookup.toJsonValue();
		break;
	}

	// Multi-select
	case MFiles::Constants::MultiSelectLookup :
	{
		QJsonArray lookups;
		lookups.push_back( lookup.toJsonValue() );
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
	// Delegate.
	this->setMultiSelectLookup( lookups );
}

TypedValue::TypedValue( const TypedValue& source ) :
	MFilesTypeWrapper( source.toJsonValue() )
{

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
		lookups.push_back( this->object()[ "Lookup" ] );
		break;
	}

	// Multi-select
	case 10:
	{
		foreach( Lookup lookup, this->object()[ "Lookups" ].toArray() )
		{
			lookups.push_back( lookup.toJsonValue() );
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
QSet< int > TypedValue::getLookupIds() const
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

void TypedValue::setMultiSelectLookup( const QJsonArray& lookups )
{
	// Get reference to the internal typed value variable and set the values.
	QJsonObject& typedValue = this->object();
	typedValue[ "DataType" ] = MFiles::Constants::MultiSelectLookup;
	typedValue[ "HasValue" ] = lookups.size() != 0;
	typedValue[ "DisplayValue" ] = QString( "Demo" );
	typedValue[ "Lookups" ] = lookups;
	typedValue[ "Value" ] = lookups;
	Q_ASSERT( this->hasValue() || lookups.count() == 0 );
	Q_ASSERT( this->asLookups().count() == lookups.count() );
}

bool TypedValue::dropLookupsExcept( const QSet< int > allowedLookups )
{
	// Stop if we do not have a value.
	if( ! this->hasValue() )
		return false;

	bool somethingDropped = false;
	QJsonObject& typedValue = this->object();
	switch ( this->dataType() )
	{
	// Single-select.
	case MFiles::Constants::SingleSelectLookup :
	{
		// Drop the only lookup if not included in allowedLookups.
		Lookup lookup( typedValue[ "Lookup" ] );
		if( ! allowedLookups.contains( lookup.item() ) )
		{
			// The lookup value should be dropped.
			somethingDropped = true;
			typedValue[ "Lookup" ] = QJsonValue::Null;
			typedValue[ "Value" ] = QJsonValue::Null;
			typedValue[ "DisplayValue" ] = QString( "" );
			typedValue[ "HasValue" ] = false;
		}
		break;
	}

	// Multi-select
	case MFiles::Constants::MultiSelectLookup:
	{
		// Get current lookups and drop all those lookup values that are not allowed.
		// Then set them back to this typed value.
		QJsonArray lookups = typedValue[ "Lookups" ].toArray();
		for( QJsonArray::iterator itr = lookups.begin();
			 itr != lookups.end();  )
		{
			// Discard all lookups that are not allowed.
			Lookup lookup( *itr );
			if( allowedLookups.contains( lookup.item() ) )
			{
				// This lookup is allowed.
				itr++;
			}
			else
			{
				// This lookup is not allowed. Erase it.
				somethingDropped = true;
				itr = lookups.erase( itr );
			}

		}
		this->setMultiSelectLookup( lookups );
		break;
	}

	// Unexpected data type.
	default:
		qCritical( "TODO: Report error." );
		break;
	}

	return somethingDropped;
}

QString TypedValue::getUriEncodedValue() const
{
	// Uninitialized?
	if( ! hasValue() )
		return "-";

	// Determine the encoding prefix and the encoded value.
	char prefix;
	QString value = "";
	switch ( this->dataType() )
	{
	case MFiles::Constants::Text :
		prefix = 'T';
		value = QUrl::toPercentEncoding( this->getAsUnlocalizedText() ).constData();
		break;

	case MFiles::Constants::Integer :
		prefix = 'I';
		value = QString::number( this->value().toDouble() );
		break;

	case MFiles::Constants::Floating :
		prefix = 'R';
		value = QString::number( this->value().toDouble() );
		break;

	case MFiles::Constants::Date :
		{
			prefix = 'D';
			QDate date = QDate::fromString( this->value().toString(), Qt::ISODate );
			value = date.toString( "yyyy-MM-dd" );
		}
		break;

	// TODO: Need to investagte how to properly format the time and timestamps fields.
	/*case MFiles::Constants::Time :
		{
			prefix = 'C';
			qDebug() << this->value().toString();
			QDateTime dt = QDateTime::fromString( this->value().toString(), Qt::ISODate );
			value = dt.toString( "yyyy-MM-ddThh:mm:ss" );
		}
		break;

	case MFiles::Constants::Timestamp :
		prefix = 'P';
		value = this->value().toString();
		break;*/

	case MFiles::Constants::SingleSelectLookup :
		prefix = 'L';
		value = QString::number( this->lookup().item() );
		break;

	// Multi-select
	case MFiles::Constants::MultiSelectLookup:
		prefix = 'S';
		foreach( int lookupId, this->getLookupIds() )
		{
			value.append( QString::number( lookupId ) );
			value.append( ',' );
		}
		value.chop( 1 );  // Remove the extra comma.
		break;

	case MFiles::Constants::MultiLineText :
		prefix = 'M';
		value = QUrl::toPercentEncoding( this->getAsUnlocalizedText() ).constEnd();
		break;

	// Unexpected data type.
	default:
		prefix = '-';
		qCritical( "TODO: Report error." );
		break;
	}

	// Format the value.
	QString format( "%1%2" );
	QString encodedValue = format.arg( prefix ).arg( value );

	// Double URI encode.
	encodedValue = QUrl::toPercentEncoding( encodedValue );
	// encodedValue.replace( "%", "_p" );
	return encodedValue;
}

QString TypedValue::getAsUnlocalizedText() const
{
	// Uninitialized?
	if( ! hasValue() )
		return "";

	QString value;
	switch ( this->dataType() )
	{
	case MFiles::Constants::Text :
	case MFiles::Constants::MultiLineText :
		value = this->property( "Value" ).toString();
		break;

	// Unexpected data type.
	default:
		qCritical( "TODO: Report error." );
		break;
	}
	return value;
}

}
