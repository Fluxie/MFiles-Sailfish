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

#include "../mfiles/lookup.h"
#include "../mfiles/typedvalue.h"

#include "valuelistcore.h"

#include "vaultcore.h"

ValueListCore::ValueListCore(VaultCore* vault, int valueList, int owner ) :
	StructureCacheBase(
		QString( "/valuelists/%1/items?page" ).arg( valueList ),
		QString( "/valuelists/%1/items/%2?page" ).arg( valueList ).arg( "%1" ),
		vault, false ),
	m_valueList( valueList ),
	m_owner( owner ),
	m_filter( 0 )
{
}

//! Constructs value list object to represent one value list accessed via the specified filter.
ValueListCore::ValueListCore( VaultCore* vault, int valueList, int owner, const TypedValueFilter* filter ) :
	StructureCacheBase(
		ValueListCore::getResource( true, valueList, owner, filter ),
		ValueListCore::getResource( false, valueList, owner, filter ),
		vault, true ),
	m_valueList( valueList ),
	m_owner( owner ),
	m_filter( new TypedValueFilter( *filter, 0 ) )  // Do not specify us as the parent as we may be called from a different thread.
{
	// Push the filter object to correct thread.
	m_filter->moveToThread( this->thread() );
}

//! Constructs value list object to represent one value list accessed via the specified filter.
ValueListCore::ValueListCore(
		const QString& resource, const QString& fetchOne,
		VaultCore* vault, int valueList, int owner, const TypedValueFilter* filter ) :
	StructureCacheBase(	resource, fetchOne, vault, true ),
	m_valueList( valueList ),
	m_owner( owner ),
	m_filter( new TypedValueFilter( *filter, 0 ) )  // Do not specify us as the parent as we may be called from a different thread.
{
	// Push the filter object to correct thread.
	m_filter->moveToThread( this->thread() );
}

//! Destructor.
ValueListCore::~ValueListCore()
{
	// Cleanup the filter.
	delete m_filter;
}

/**
 * @brief Normalizes the presentation of the value.
 * @param value The value that is normalized.
 */
QJsonValue ValueListCore::normalizeValue( QJsonValue value )
{
	// Normalize the value list item.
	QJsonObject asObject = value.toObject();
	if( ! asObject.contains( "HasOwner") )
		asObject[ "HasOwner" ] = false;
	if( ! asObject.contains( "CanSelect") )  // CanSelect is not returned by MFWS REST API. Instead it is used internally by this framework.
		asObject[ "CanSelect" ] = true;
	value = asObject;
	return value;
}

QString ValueListCore::getResource( bool allItems, int valueList, int owner, const TypedValueFilter* filter )
{
	Q_CHECK_PTR( filter );

	// Construct the resource for fetching the value list items and apply relevant filters.
	QString resource;
	if( allItems )
		resource = QString( "/valuelists/%1/items?page" ).arg( valueList );
	else
		resource = QString( "/valuelists/%1/items/%2?page" ).arg( valueList ).arg( "%1" );
	if( filter->propertyDef() != TypedValueFilter::Undefined )
		resource.append( QString( "&propertydef=%1" ).arg( filter->propertyDef() ) );
	MFiles::TypedValue ownerInfo( filter->ownerInfo() );
	if( ! ownerInfo.isUndefined() && ownerInfo.hasValue() )
	{
		qDebug( "Constructirg owner filter" );
		QString items;
		foreach( MFiles::Lookup lookup, ownerInfo.asLookups() )
		{
			// Add separater if there are already items.
			if( ! items.isEmpty() )
				items.append( ',' );

			// Append the actual item.
			items.append( QString::number( lookup.item() ) );

		}  // end foreach.
		Q_ASSERT( ! items.isEmpty() );
		qDebug( items.toLatin1() );

		// Append the item list to create the final filter query parameters.
		resource.append( QString( "&filterItem=%1:%2" ).arg( owner ).arg( items ) );
	}
	qDebug( resource.toLatin1() );
	return resource;
}
