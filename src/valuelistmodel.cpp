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
QVariant ValueListModel::data ( const QModelIndex& index, int role ) const
{
	switch( role )
	{
	case Qt::DisplayRole :
		return forDisplay( index );

	case Qt::DecorationRole :
		return forDecoration( index );

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
		return QVariant();
	}
}

//! Called when the reset of the model is required.
void ValueListModel::resetFromList()
{
	qDebug( "Resetting model..." );
	this->beginResetModel();
	if( m_valueList )
		m_data = m_valueList->items();
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
	emit valueListChanged();
	this->endResetModel();
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
