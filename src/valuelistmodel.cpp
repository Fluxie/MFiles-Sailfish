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

#include "valuelistmodel.h"

//! The role id the lookup role.
const int ValueListModel::LookupRole = Qt::UserRole;

//! The role id the id role.
const int ValueListModel::IdRole = Qt::UserRole + 1;

ValueListModel::ValueListModel() :
	QAbstractListModel(),
	m_valueList( 0 )
{
}

//! Returns the number of rows under the given parent.
int ValueListModel::rowCount( const QModelIndex& parent ) const
{
	// Report the row count.
	int rowCount = m_data.size();
	return rowCount;
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant ValueListModel::data( const QModelIndex& index, int role ) const
{
	switch( role )
	{
	case Qt::DisplayRole :
		return forDisplay( index );

	case Qt::DecorationRole :
		return forDecoration( index );

	case ValueListModel::LookupRole :
		return forLookup( index );

	case ValueListModel::IdRole :
		return forId( index );

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
		return QVariant();
	}
}

//! Role names. Note: The documentation claims that we should call setRoleNames to specify the roles. However, this function no longer exists and roleNAmes has been made virtula.
QHash< int,QByteArray > ValueListModel::roleNames() const
{
	// Construct QHash to describe the roles and return it.
	// TODO: Should we reset the original roles too here?
	QHash< int, QByteArray > roles;
	roles.insert( Qt::DisplayRole, QString( "display" ).toLatin1() );
	roles.insert( Qt::DecorationRole, QString( "decoration" ).toLatin1() );
	roles.insert( ValueListModel::LookupRole, QString( "lookup" ).toLatin1() );
	roles.insert( ValueListModel::IdRole, QString( "id" ).toLatin1() );
	return roles;
}

//! Called when the reset of the model is required.
void ValueListModel::resetFromList()
{
	qDebug( "Resetting model..." );
	this->beginResetModel();
	if( m_valueList )
	{
		m_data = m_valueList->items();
		this->includeSelectedLookupIfMissing( false );
	}
	else
		m_data = QJsonArray();
	this->endResetModel();
}

//! Sets the value list. This also resets the model.
void ValueListModel::setValueList( ValueListFront* valueList )
{
	// Sanity check.
	if( m_valueList != 0 )
		qCritical( "Changing the value list of value list model is not supported." );
	if( m_valueList == valueList )
		return;  // Do nothing, as nothing has changed.

	// Changing the value list resets the model.
	this->beginResetModel();
	m_valueList = valueList;
	m_data = m_valueList->items();
	QObject::connect( m_valueList, &ValueListFront::statusChanged, this, &ValueListModel::resetFromList );

	// Include the selected lookup in the list if it is missing.
	this->includeSelectedLookupIfMissing( false );

	emit valueListChanged();
	this->endResetModel();
}

//! Sets the currently selected lookup.
void ValueListModel::setSelectedLookup( const QJsonValue& lookup )
{
	// Skip if the lookup does not change.
	if( m_selectedLookup == lookup )
		return;

	// Set.
	m_selectedLookup = lookup;
	this->includeSelectedLookupIfMissing( true );
	emit selectedLookupChanged();
}

//! Includes the selected lookup in the data if it is missing.
void ValueListModel::includeSelectedLookupIfMissing( bool notify )
{
	// Add the lookup as value list item if the current listing does not include it.
	QJsonValue& lookup = m_selectedLookup;
	if( ! lookup.isNull() && lookup.isUndefined() )
	{
		// Add.
		int lookupId = lookup.toObject()[ "ID" ].toDouble();
		int index = this->indexOf( lookupId );
		if( index == -1 )
			this->insertLookup( lookup, notify );
		Q_ASSERT( this->indexOf( lookupId ) != -1 );

	}  // end if.
}

//! Gets the index of the item in the stored value list item or -1 if the item does not exist.
int ValueListModel::indexOf( int id ) const
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
void ValueListModel::insertLookup( const QJsonValue& lookup, bool notify )
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

		}  // end if

	}  // end for.

	// Append the value to the end of the value list if it was not already inserted.
	if( ! inserted )
		this->insertLookup( this->m_data.size(), lookup, notify );
}

//! Inserts lookup to the value list to the specified position.
void ValueListModel::insertLookup( int row, const QJsonValue& lookup, bool notify )
{
	// Construct the value list item based on the lookup.
	QJsonObject lookupObject = lookup.toObject();
	QJsonObject vlitemToInsert;
	vlitemToInsert[ "DisplayID" ] = lookupObject[ "ID" ].toString();
	vlitemToInsert[ "HasOwner" ] = false;
	vlitemToInsert[ "HasParent" ] = false;
	vlitemToInsert[ "ID" ] = lookupObject[ "ID" ];
	vlitemToInsert[ "Name" ] = lookupObject[ "DisplayValue" ];
	vlitemToInsert[ "OwnerID" ] = 0;
	vlitemToInsert[ "ParentID" ] = 0;
	vlitemToInsert[ "ValueListID" ] = m_valueList->id();

	// Make the insert.
	if( notify )
		this->beginInsertRows( this->index( row ), row, row );
	m_data.insert( row, QJsonValue( vlitemToInsert ) );
	if( notify )
		this->endInsertRows();
}

//! Returns data for display.
QVariant ValueListModel::forDisplay( const QModelIndex & index ) const
{
	QJsonValue asValue = m_data.at( index.row() );
	QString text = asValue.toObject()[ "Name" ].toString();
	return QVariant( text );
}

//! Returns data for decoration.
QVariant ValueListModel::forDecoration( const QModelIndex & index ) const
{
	qDebug( "Decoration role" );
	return QVariant();
}

//! Returns data for lookup.
QVariant ValueListModel::forLookup( const QModelIndex & index ) const
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
QVariant ValueListModel::forId( const QModelIndex & index ) const
{
	int id = m_data[ index.row() ].toObject()[ "ID" ].toDouble();
	return QVariant( id );
}
