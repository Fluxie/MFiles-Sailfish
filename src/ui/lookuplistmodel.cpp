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

#include "lookuplistmodel.h"

#include <QDebug>
#include <QJsonObject>

//! The role id of the lookup role.
const int LookupListModel::LookupRole = Qt::UserRole;

LookupListModel::LookupListModel(QObject *parent) :
	QAbstractListModel(parent),
	m_rowLimit( 0 ),
	m_dataType( 0 )
{
}

//! Returns the number of rows under the given parent.
int LookupListModel::rowCount( const QModelIndex& ) const
{
	// Report the row count.
	int rowCount = m_lookups.size();
	if( m_rowLimit != 0 && rowCount > m_rowLimit )
		rowCount = m_rowLimit;
	return rowCount;
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant LookupListModel::data( const QModelIndex& index, int role ) const
{
	// Return the role.
	QVariant data;
	switch( role )
	{
	case Qt::DisplayRole :
		this->forDisplay( index, data );
		break;

	case Qt::DecorationRole :
		this->forDecoration( index, data );
		break;

	// Lookup role.
	case LookupListModel::LookupRole :
		this->forLookup( index, data );
		break;

	default:
		qDebug() << QString( "Unknown role %1").arg( role );
	}
	return data;
}

//! Flags.
Qt::ItemFlags LookupListModel::flags( const QModelIndex &index ) const
{

	if (!index.isValid())
		 return Qt::ItemIsEnabled;

	 return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

//! Sets the data.
bool LookupListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	// Skip setting any data if we do not have any values to show.
	if( role != LookupListModel::LookupRole )
		return false;
	if( ! index.isValid() )
		return false;

	// The value given should be valid.
	Q_ASSERT( value.isValid() );

	// We can only convert variant maps at the moment.
	// JSON objects are returned as variant maps from QML.
	if( value.type() != QVariant::Map )
	{
		// TODO: Report error.
		qCritical() << value.typeName();
		return false;
	}

	// Check if the value has changed.
	const QJsonValue& previousValue = m_lookups[ index.row() ];
	QJsonValue newValue( QJsonObject::fromVariantMap( qvariant_cast< QVariantMap >( value ) ) );
	if( previousValue == newValue )
		return false;

	// The value denoted by the index has changed. Update it with the new value and signal the change.
	qDebug( "Updating lookup model data." );
	m_lookups[ index.row() ] = newValue;
	this->updatePropertyValueFromLookups();
	emit dataChanged( index, index );
	return true;
}

//! Role names.
//! Note: The documentation claims that we should call setRoleNames to specify the roles.
//! However, this function no longer exists and roleNAmes has been made virtula.
QHash< int, QByteArray > LookupListModel::roleNames() const
{
	// Construct QHash to describe the roles and return it.
	// TODO: Should we reset the original roles too here?
	QHash< int, QByteArray > roles;
	roles.insert( Qt::DisplayRole, QString( "display" ).toLatin1() );
	roles.insert( Qt::DecorationRole, QString( "decoration" ).toLatin1() );
	roles.insert( LookupListModel::LookupRole, QString( "lookup" ).toLatin1() );
	return roles;
}

//! Sets the maximum number of rows to display.
void LookupListModel::setRowLimit( int rowLimit )
{
	// Skip if nothing is changing.
	if( m_rowLimit == rowLimit )
		return;

	// Update the row limit.
	this->beginResetModel();
	{
		m_rowLimit = rowLimit;
	}
	this->endResetModel();

	// Row limit was changed.
	emit rowLimitChanged();
}

//! Sets property value.
void LookupListModel::setPropertyValue( const QJsonValue propertyValue )
{
	// Don't update if the property value has not changed.
	if( m_propertyValue == propertyValue )
		return;

	// Extract lookups.
	int oldLookupCount = this->lookupCount();
	QJsonObject asObject = propertyValue.toObject();
	QJsonObject typedValue = asObject[ "TypedValue" ].toObject();
	m_dataType = typedValue[ "DataType" ].toDouble();
	bool hasValue = typedValue[ "HasValue" ].toBool();
	QJsonArray lookups;
	if( m_dataType == 9 && hasValue )
	{
		// Single-select lookup.
		lookups.append( typedValue[ "Lookup" ] );
	}
	else if( m_dataType == 10 && hasValue )
	{
		// Multi-select lookup.
		lookups = typedValue[ "Lookups" ].toArray();
	}
	else if( ! hasValue )
	{
		// No value.
	}
	else
	{
		// Unexpected data type.
		qCritical( "TODO: Error reporting." );
		return;
	}

	// Update the model data.
	this->beginResetModel();
	{
		m_propertyValue = propertyValue;
		m_lookups = lookups;
	}
	this->endResetModel();

	// Lookup count changed?
	if( oldLookupCount != this->lookupCount() )
		emit lookupCountChanged();

	// Property value was changed.
	emit propertyValueChanged();
}

//! Updates the property value from the current lookups.
void LookupListModel::updatePropertyValueFromLookups()
{
	// Make the update.
	QJsonObject pvAsObject = m_propertyValue.toObject();
	QJsonObject typedValue = pvAsObject[ "TypedValue" ].toObject();
	switch( m_dataType )
	{
	// Single-select lookup.
	case 9:
		if( m_lookups.count() > 0 )
			typedValue[ "Lookup" ] = m_lookups[ 0 ];
		else
			typedValue[ "Lookup" ] = QJsonValue();
		break;

	// Multi-select lookups
	case 10 :
		typedValue[ "Lookups" ] = m_lookups;
		break;

	// Unexpected property value.
	default:
		qCritical( "TODO: Error reporting" );
		break;
	}
	pvAsObject[ "TypedValue" ] = typedValue;
	m_propertyValue = pvAsObject;

	// The property value was changed.
	emit propertyValueChanged();
}

//! Returns data for display.
void LookupListModel::forDisplay( const QModelIndex & index, QVariant& variant ) const
{
	// Set the value.
	QJsonObject asObject = m_lookups[ index.row() ].toObject();
	variant.setValue( asObject[ "DisplayValue" ].toString() );
}

//! Returns data for decoration.
void LookupListModel::forDecoration( const QModelIndex & index, QVariant& variant ) const
{
	// Nothing.
	Q_UNUSED( index )
	Q_UNUSED( variant )
}

//! Returns data for lookup.
void LookupListModel::forLookup( const QModelIndex & index, QVariant& variant ) const
{
	variant.setValue( m_lookups[ index.row() ] );
}
