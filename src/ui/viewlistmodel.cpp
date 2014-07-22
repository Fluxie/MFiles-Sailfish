#include "viewlistmodel.h"

ViewListModel::ViewListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

//! Returns the number of rows under the given parent.
int ViewListModel::rowCount( const QModelIndex& ) const
{
	// Report the row count.
	/*int rowCount = m_lookups.size();
	if( m_rowLimit != 0 && rowCount > m_rowLimit )
		rowCount = m_rowLimit;
	return rowCount;*/
	return 0;
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant ViewListModel::data( const QModelIndex& index, int role ) const
{
	// Return the role.
	QVariant data;
	/*switch( role )
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
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
	}*/
	return data;
}

//! Flags.
Qt::ItemFlags ViewListModel::flags( const QModelIndex &index ) const
{
	if (!index.isValid())
		 return Qt::ItemIsEnabled;

	 return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

//! Sets the data.
bool ViewListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	// Skip setting any data if we do not have any values to show.
	/*if( role != LookupListModel::LookupRole )
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
		qCritical( value.typeName() );
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
	emit dataChanged( index, index );*/
	return true;
}

//! Role names.
//! Note: The documentation claims that we should call setRoleNames to specify the roles.
//! However, this function no longer exists and roleNAmes has been made virtula.
QHash< int, QByteArray > ViewListModel::roleNames() const
{
	// Construct QHash to describe the roles and return it.
	// TODO: Should we reset the original roles too here?
	QHash< int, QByteArray > roles;
	roles.insert( Qt::DisplayRole, QString( "display" ).toLatin1() );
	roles.insert( Qt::DecorationRole, QString( "decoration" ).toLatin1() );
	//roles.insert( LookupListModel::LookupRole, QString( "lookup" ).toLatin1() );
	return roles;
}
