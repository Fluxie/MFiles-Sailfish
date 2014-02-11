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

#ifndef TYPEDVALUE_H
#define TYPEDVALUE_H

#include <QSet>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "lookup.h"
#include "mfilestypecapsule.h"

/**
 * @brief The TypedValue class
 */
class TypedValue : public MFilesTypeCapsule
{
public:

	/**
	 * @brief Initializes new typed value.
	 */
	TypedValue( const QJsonValue& typedValue );

	/**
	 * @brief Initializes a new lookup based typed value
	 */
	TypedValue( int dataType, const Lookup& lookup );

	/**
	 * @brief Initializes a new multi-select lookup based typed value
	 */
	TypedValue( const QJsonArray& lookups );

	//! Data type.
	int dataType() const { return this->object()[ "DataType" ].toDouble(); }

	/**
	 * @brief hasValue
	 * @return True if the typed value has a value.
	 */
	bool hasValue() const { return this->object()[ "HasValue" ].toBool(); }

	//! Lookup.
	Lookup lookup() const { return Lookup( this->object()[ "Lookup" ] ); }

	/**
	 * @brief asLookups
	 * @return All lookups as a Json array. Can be called for both single-select and multi-select lookups.
	 */
	QJsonArray asLookups() const;

	/**
	 * @brief getLookupIds
	 * @return A collection of lookup ids.
	 */
	QSet< int > getLookupIds();

	/**
	 * @brief dropLookupsExcept Drops all lookups from this typed value that are not in the specified set.
	 * @param allowedLookups Ids of lookups that are allowed.
	 * @return Returns true if something was dropped.
	 */
	bool dropLookupsExcept( const QSet< int > allowedLookups );

	/**
	 * @brief setMultiSelectLookup Sets lookups.
	 * @param lookups Lookup values.
	 */
	void setMultiSelectLookup( const QJsonArray& lookups );

};

inline uint qHash( const TypedValue& typedValue )
{
	uint hash = typedValue.dataType();
	hash = typedValue.hasValue() ? ~hash : hash;
	qDebug( "TypedValue hash" );
	foreach( Lookup lookup, typedValue.asLookups() )
	{
		qDebug( "Lookup for hash." );
		hash ^= qHash( lookup );
	}
	return hash;
}

#endif // TYPEDVALUE_H
