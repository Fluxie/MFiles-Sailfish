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

#include "valuelistitemlistmodel.h"

#include "asyncfetch.h"
#include "mfiles/lookup.h"
#include "mfiles/valuelistitem.h"

//! The role id the lookup role.
const int ValueListItemListModel::LookupRole = Qt::UserRole;

//! The role id the id role.
const int ValueListItemListModel::IdRole = Qt::UserRole + 1;

ValueListItemListModel::ValueListItemListModel() :
	QAbstractListModel(),
	m_valueList( 0 )
{
	qDebug( "ValueListItemListModel constructed." );
}

//! Blocked lookups
QJsonArray ValueListItemListModel::blockedLookups() const
{
	// Collect and return the blocked lookups.
	QJsonArray blocked;
	QList< QJsonValue > blockedAsList = m_blockedLookups.values();
	foreach( const QJsonValue& bl, blockedAsList )
	{
		blocked.push_back( bl );
	}
	return blocked;
}

//! Returns the number of rows under the given parent.
int ValueListItemListModel::rowCount( const QModelIndex& parent ) const
{
	// Report the row count.
	int rowCount = m_data.size();
	return rowCount;
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant ValueListItemListModel::data( const QModelIndex& index, int role ) const
{
	switch( role )
	{
	case Qt::DisplayRole :
		return forDisplay( index );

	case Qt::DecorationRole :
		return forDecoration( index );

	case ValueListItemListModel::LookupRole :
		return forLookup( index );

	case ValueListItemListModel::IdRole :
		return forId( index );

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
		return QVariant();
	}
}

//! Role names. Note: The documentation claims that we should call setRoleNames to specify the roles. However, this function no longer exists and roleNAmes has been made virtula.
QHash< int,QByteArray > ValueListItemListModel::roleNames() const
{
	// Construct QHash to describe the roles and return it.
	// TODO: Should we reset the original roles too here?
	QHash< int, QByteArray > roles;
	roles.insert( Qt::DisplayRole, QString( "display" ).toLatin1() );
	roles.insert( Qt::DecorationRole, QString( "decoration" ).toLatin1() );
	roles.insert( ValueListItemListModel::LookupRole, QString( "lookup" ).toLatin1() );
	roles.insert( ValueListItemListModel::IdRole, QString( "id" ).toLatin1() );
	return roles;
}

//! Called when the reset of the model is required.
void ValueListItemListModel::resetFromList()
{
	// Reset the values from the value list.
	if( m_valueList )
	{
		// Fetch the items.
		AsyncFetch* fetch = this->filterBlocked( m_valueList->items() );
		fetch->setParent( this );
		QObject::connect( fetch, &AsyncFetch::finished, [=]() {
			fetch->deleteLater();

			// Set the data.
			this->setData( fetch->values() );
		} );
	}
	else
	{
		// No value list available.
		this->setData( QJsonArray() );
	}
}

//! Sets the value list. This also resets the model.
void ValueListItemListModel::setValueList( ValueListFront* valueList )
{
	// Sanity check.
	if( m_valueList != 0 )
		qCritical( "Changing the value list of value list model is not supported." );
	if( m_valueList == valueList )
		return;  // Do nothing, as nothing has changed.	

	// Disconnect the status changed signal of the previous value list.
	if( m_valueList )
		QObject::disconnect( m_valueList, &ValueListFront::statusChanged, this, &ValueListItemListModel::resetFromList );

	// Store the value list and items.
	if( valueList != 0 )
	{
		// Store the value list.
		m_valueList = valueList;
		QObject::connect( m_valueList, &ValueListFront::statusChanged, this, &ValueListItemListModel::resetFromList );

		// Reset data from the list.
		this->resetFromList();
	}
	else
	{
		// Clear the previous value list.
		m_valueList = 0;
		this->setData( QJsonArray() );
	}
}

//! Sets the filter.
void ValueListItemListModel::setFilter( TypedValueFilter* filter )
{
	qDebug( "ValueListItemListModel" );
	// Skip if the filter isn't changed.
	if( m_filter == filter )
		return;

	// Change the filter.
	m_filter = filter;
	this->resetFromList();
	emit filterChanged();

	qDebug( "ValueListItemListModel" );
}

//! Sets the currently selected lookup.
void ValueListItemListModel::setSelectedLookup( const QJsonValue& lookup )
{
	qDebug( "ValueListItemListModel" );
	// Skip if the lookup does not change.
	if( m_selectedLookup == lookup )
		return;

	// Set.
	m_selectedLookup = lookup;
	this->includeSelectedLookupIfMissing( true );
	emit selectedLookupChanged();
	qDebug( "ValueListItemListModel" );
}

//! Sets the blocked lookups.
void ValueListItemListModel::setBlockedLookups( const QJsonArray& blocked )
{
	qDebug( "ValueListItemListModel" );
	// Set the blocked lookups and construct a blocking list from them.
	m_blockedLookups.clear();
	for( QJsonArray::const_iterator itr = blocked.constBegin(); itr != blocked.constEnd(); itr++ )
	{
		// Store the blocked lookups.
		QJsonObject asObject = (*itr).toObject();
		int itemId = asObject[ "Item" ].toDouble();
		m_blockedLookups.insert( itemId, (*itr) );
	}	
	this->resetFromList(); // Reset after blocking.	
	emit blockedLookupsChanged();
	qDebug( "ValueListItemListModel" );
}

//! Returns an array of value list items without the blocked lookups.
AsyncFetch* ValueListItemListModel::filterBlocked( AsyncFetch* items ) const
{
	qDebug( "ValueListItemListModel" );
	// Filter the items if necessary.
	if( m_blockedLookups.empty() )
	{
		// No need to add a separate filter.
	}
	else
	{
		// Set filter that prevents blocked lookups.
		items->appendFilter( [=]( const QJsonValue& input ) -> bool {

			// Check if the current item should be blocked.
			MFiles::ValueListItem item( input );
			bool blocked =  m_blockedLookups.contains( item.id() );
			return ! blocked;

		} );

	}  // end if

	return items;
}

//! Includes the selected lookup in the data if it is missing.
void ValueListItemListModel::includeSelectedLookupIfMissing( bool notify )
{
	// Cannot include / no need to include yet, the value list is not available.
	if( m_valueList == 0 )
		return;

	// Add the lookup as value list item if the current listing does not include it.
	MFiles::Lookup lookup( m_selectedLookup );
	if( ! lookup.isUndefined() )
	{
		// Add.
		int lookupId = lookup.item();
		int index = this->indexOf( lookupId );
		if( index == -1 )
			this->insertLookup( lookup.value(), notify );
		Q_ASSERT( this->indexOf( lookupId ) != -1 );

	}  // end if.
}

//! Gets the index of the item in the stored value list item or -1 if the item does not exist.
int ValueListItemListModel::indexOf( int id ) const
{
	// Search for the specified lookup and return the index if found.	
	for( int i = 0; i < m_data.size(); i++ )
	{
		// Check if the current item is the one we are looking for.
		QJsonObject object =  m_data[ i ].toObject();
		int itemId = object[ "ID" ].toDouble();
		if( itemId == id )
			return i;

	}  // end for

	// The item was not found.	
	return -1;
}

//! Inserts lookup to the value list.
void ValueListItemListModel::insertLookup( const QJsonValue& lookup, bool notify )
{	
	// Search for position to insert the object.	
	bool inserted = false;
	QString lookupDislayValue = lookup.toObject()[ "DisplayValue" ].toString();
	for( int before = 0; before < m_data.size(); before++ )
	{
		// Check if the lookup we are inserting should come before the current value list item.
		QJsonObject asObject = m_data[ before ].toObject();
		QString beforeDisplayValue = asObject[ "Name" ].toString();
		if( lookupDislayValue.compare( beforeDisplayValue, Qt::CaseInsensitive ) < 0 )
		{
			// Make the insert.			
			this->insertLookup( before, lookup, notify );
			inserted = true;
			break;

		}  // end if

	}  // end for.

	// Append the value to the end of the value list if it was not already inserted.		
	if( ! inserted )
		this->insertLookup( this->m_data.size(), lookup, notify );		
}

//! Inserts lookup to the value list to the specified position.
void ValueListItemListModel::insertLookup( int row, const QJsonValue& lookup, bool notify )
{
	// Cannot insert if value list is not yet available.
	Q_CHECK_PTR( m_valueList );

	// Construct the value list item based on the lookup.
	MFiles::Lookup asLookup( lookup );
	QJsonObject vlitemToInsert;
	vlitemToInsert[ "DisplayID" ] = QString( asLookup.item() );
	vlitemToInsert[ "HasOwner" ] = false;
	vlitemToInsert[ "HasParent" ] = false;
	vlitemToInsert[ "ID" ] = asLookup.item();
	vlitemToInsert[ "Name" ] = asLookup.displayValue();
	vlitemToInsert[ "OwnerID" ] = 0;
	vlitemToInsert[ "ParentID" ] = 0;
	vlitemToInsert[ "ValueListID" ] = m_valueList->id();

	// Make the insert if the item is not blocked.
	int id = vlitemToInsert[ "ID" ].toDouble();
	if( ! m_blockedLookups.contains( id ) )
	{
		if( notify )
			this->beginInsertRows( this->index( row ), row, row );
		m_data.insert( row, QJsonValue( vlitemToInsert ) );
		if( notify )
			this->endInsertRows();
	}
}

void ValueListItemListModel::setData( const QJsonArray& data )
{
	this->beginResetModel();
	{
		// Store the data.
		m_data = data;

		// Include the selected lookup in the list if it is missing.
		this->includeSelectedLookupIfMissing( false );
		emit valueListChanged();
	}
	this->endResetModel();
}

//! Returns data for display.
QVariant ValueListItemListModel::forDisplay( const QModelIndex & index ) const
{
	QJsonValue asValue = m_data.at( index.row() );
	QString text = asValue.toObject()[ "Name" ].toString();
	return QVariant( text );
}

//! Returns data for decoration.
QVariant ValueListItemListModel::forDecoration( const QModelIndex & index ) const
{
	qDebug( "Decoration role" );
	return QVariant();
}

//! Returns data for lookup.
QVariant ValueListItemListModel::forLookup( const QModelIndex & index ) const
{
	// Construct lookup based on the value list item denoted by the index.
	QJsonObject vlitem = m_data[ index.row() ].toObject();
	QJsonObject lookup;
	lookup[ "Deleted" ] = false;
	lookup[ "DisplayValue" ] = vlitem[ "Name" ];
	lookup[ "Hidden" ] = false;
	lookup[ "Item" ] = vlitem[ "ID" ];
	lookup[ "Version" ] = -1;  // The latest version.

	// Return the data.
	return QVariant( QJsonValue( lookup ) );
}

//! Returns data for Id.
QVariant ValueListItemListModel::forId( const QModelIndex & index ) const
{
	int id = m_data[ index.row() ].toObject()[ "ID" ].toDouble();
	return QVariant( id );
}
