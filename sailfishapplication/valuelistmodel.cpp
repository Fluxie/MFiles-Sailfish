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
	qDebug( QString( "Display role %1" ).arg( text ).toLatin1() );
	return QVariant( text );
}

//! Returns data for decoration.
QVariant ValueListModel::forDecoration( const QModelIndex & index ) const
{
	qDebug( "Decoration role" );
	return QVariant();
}
