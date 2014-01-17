#include "lookupmodel.h"

#include <QJsonObject>

LookupModel::LookupModel(QObject *parent) :
	QAbstractListModel(parent),
	m_rowLimit( 0 )
{
}

//! Returns the number of rows under the given parent.
int LookupModel::rowCount( const QModelIndex& ) const
{
	// Report the row count.
	int rowCount = m_lookups.size();
	if( rowCount > m_rowLimit )
		rowCount = m_rowLimit;
	return rowCount;
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant LookupModel::data( const QModelIndex& index, int role ) const
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

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
	}
	return data;
}

//! Sets the maximum number of rows to display.
void LookupModel::setRowLimit( int rowLimit )
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
void LookupModel::setPropertyValue( const QJsonValue propertyValue )
{
	// Don't update if the property value has not changed.
	if( m_propertyValue == propertyValue )
		return;

	// Extract lookups.
	int oldLookupCount = this->lookupCount();
	QJsonObject asObject = propertyValue.toObject();
	QJsonObject typedValue = asObject[ "TypedValue" ].toObject();
	int dataType = typedValue[ "DataType" ].toDouble();
	bool hasValue = typedValue[ "HasValue" ].toBool();
	QJsonArray lookups;
	if( dataType == 9 && hasValue )
	{
		// Single-select lookup.
		lookups.append( typedValue[ "Lookup" ] );
	}
	else if( dataType == 10 && hasValue )
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

//! Returns data for display.
void LookupModel::forDisplay( const QModelIndex & index, QVariant& variant ) const
{
	// Set the value.
	QJsonObject asObject = m_lookups[ index.row() ].toObject();
	variant.setValue( asObject[ "DisplayValue" ].toString() );
}

//! Returns data for decoration.
void LookupModel::forDecoration( const QModelIndex & index, QVariant& variant ) const
{
	// Nothing.
}
