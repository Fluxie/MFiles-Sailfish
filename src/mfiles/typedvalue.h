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
#include "mfilestypewrapper.h"

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * @brief Provides accessors for properties of TypedValue M-Files REST API Json object.
 *
 * @see <a href="http://www.m-files.com/mfws/structs/typedvalue.html">TypedValue</a> in M-Files REST API documentation.
 */
class TypedValue : public MFilesTypeWrapper
{
public:

	/**
	 * @brief Initializes typed value based on existing Json object.
	 * @param typedValue The wrapped Json object.
	 */
	TypedValue( const QJsonValue& typedValue );

	/**
	 * @brief Initializes a new lookup based typed value
	 * @param dataType Data type of the typed value. Must be single-select lookup or multi-select lookup.
	 * @param lookup First or only lookup for this typed value.
	 */
	TypedValue( int dataType, const Lookup& lookup );

	/**
	 * @brief Initializes a new multi-select lookup based typed value
	 * @param lookups Lookups for the typed value.
	 */
	TypedValue( const QJsonArray& lookups );

	/**
	 * @brief Gets the data type.
	 * @return The data type of the typed value.
	 */
	int dataType() const { return this->property( "DataType" ).toDouble(); }

	/**
	 * @brief hasValue
	 * @return True if the typed value has a value.
	 */
	bool hasValue() const { return this->property( "HasValue" ).toBool(); }

	//! Lookup.
	Lookup lookup() const { return Lookup( this->property( "Lookup" ) ); }

	/**
	 * @brief Gets the lookups of this typed value as a QJsonArray.
	 * @return All lookups as a Json array. Can be called for both single-select and multi-select lookups.
	 */
	QJsonArray asLookups() const;

	/**
	 * @brief Gets the ids of the lookups. Can be called for single-select or multi-select lookups.
	 * @return A collection of lookup ids.
	 */
	QSet< int > getLookupIds() const;

	/**
	 * @brief Drops all lookups from this typed value that are not in the specified set of lookup ids.
	 * @param allowedLookups Ids of lookups that are allowed.
	 * @return Returns true if something was dropped.
	 */
	bool dropLookupsExcept( const QSet< int > allowedLookups );

	/**
	 * @brief Sets this typed value to multi-select lookup.
	 * @param lookups Lookup values.
	 */
	void setMultiSelectLookup( const QJsonArray& lookups );

	/**
	 * @brief Gets the value as URI encoded value.
	 * @return The encoded value of this typed value.
	 *
	 * @see <a href="http://http://www.m-files.com/mfws/syntax.html">Encoding syntax</a> in M-Files REST API documentation.
	 */
	QString getUriEncodedValue() const;

	/**
	 * @brief Gets the value of this typed value as unlocalized text.
	 * @return The value of this typed value as unlocalized text.
	 */
	QString getAsUnlocalizedText() const;
};

}

/**
 * @brief Hash function for TypedValue object.
 * @param typedValue The TypedValue object for which the hash value is calculated.
 * @return Hash value of the TypedValue object.
 */
inline uint qHash( const MFiles::TypedValue& typedValue )
{
	uint hash = typedValue.dataType();
	hash = typedValue.hasValue() ? ~hash : hash;	
	foreach( MFiles::Lookup lookup, typedValue.asLookups() )
	{
		hash ^= qHash( lookup );
	}
	return hash;
}

#endif // TYPEDVALUE_H
