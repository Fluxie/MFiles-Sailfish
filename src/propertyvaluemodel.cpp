#include "propertyvaluemodel.h"

#include <QHash>
#include <QVariantMap>

#include "objectversionfront.h"


//! The role id of the property definition id role.
const int PropertyValueModel::PropertyDefinitionIdRole = Qt::UserRole;

//! The role id the property value role.
const int PropertyValueModel::PropertyValueRole = Qt::UserRole + 1;

PropertyValueModel::PropertyValueModel(QObject *parent) :
	QAbstractListModel(parent),
	m_filter( Undefined ),
	m_objectVersion( 0 )
{
}

//! Returns the number of rows under the given parent.
int PropertyValueModel::rowCount( const QModelIndex& ) const
{
	// Report the row count.
	int rowCount = m_propertyValues.size();
	return rowCount;
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant PropertyValueModel::data( const QModelIndex& index, int role ) const
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

	// Property definition id.
	case PropertyValueModel::PropertyDefinitionIdRole :
		this->forPropertyDefinitionId( index, data );
		break;

	// Property value.
	case PropertyValueModel::PropertyValueRole :
		this->forPropertyValue( index, data );
		break;

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
	}
	return data;
}

//! Flags.
Qt::ItemFlags PropertyValueModel::flags( const QModelIndex &index ) const
{

	if (!index.isValid())
		 return Qt::ItemIsEnabled;

	 return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

//! Sets the data.
bool PropertyValueModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	// Skip setting any data if we do not have any values to show.
	if( ! m_objectVersion || m_propertyValues.size() == 0 )
		return false;
	if( role != PropertyValueRole )
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
	const QJsonValue& previousValue = m_propertyValues[ index.row() ];	
	QJsonValue newValue( QJsonObject::fromVariantMap( qvariant_cast< QVariantMap >( value ) ) );
	if( previousValue == newValue )
		return false;

	// The value denoted by the index has changed. Update it with the new value and signal the change.
	qDebug( "Updating property value model data." );
	m_propertyValues[ index.row() ] = newValue;
	emit dataChanged( index, index );
	return true;
}

//! Role names.
//! Note: The documentation claims that we should call setRoleNames to specify the roles.
//! However, this function no longer exists and roleNAmes has been made virtula.
QHash< int, QByteArray > PropertyValueModel::roleNames() const
{
	// Construct QHash to describe the roles and return it.
	// TODO: Should we reset the original roles too here?
	QHash< int, QByteArray > roles;
	roles.insert( PropertyValueModel::PropertyDefinitionIdRole, QString( "propertyDefinitionId" ).toLatin1() );
	roles.insert( PropertyValueModel::PropertyValueRole, QString( "propertyValue" ).toLatin1() );
	return roles;
}

//! Sets the modelled data.
void PropertyValueModel::setDataFilter( DataFilter filter )
{
	// Nothing to do if the filter was not actually changed.
	if( filter == m_filter )
		return;

	// Specifying new filter resets the model
	this->beginResetModel();
	{
		// Select the property values we want to show based on the filter.
		m_filter = filter;
		this->refreshPropertyValues();
	}
	this->endResetModel();

	// The filter was changed.
	emit dataFilterChanged();
}

//! Sets the object version for the model.
void PropertyValueModel::setObjectVersion( ObjectVersionFront* objectVersion )
{
	// Nothing to do?
	if( m_objectVersion == objectVersion )
		return;

	// Disconnect us from the previous object version.
	if( m_objectVersion )
	{
		// Disconnect
		QObject::disconnect( m_objectVersion, &ObjectVersionFront::objectVersionChanged, this, &PropertyValueModel::refreshPropertyValues );
		QObject::disconnect( m_objectVersion, &ObjectVersionFront::propertiesForDisplayChanged, this, &PropertyValueModel::refreshPropertyValues );
		QObject::disconnect( m_objectVersion, &ObjectVersionFront::propertiesChanged, this, &PropertyValueModel::refreshPropertyValues );
	}

	// Specifying new object version resets the model.
	this->beginResetModel();
	{
		// Update the object version.
		m_objectVersion = objectVersion;
		if( m_objectVersion )
		{
			QObject::connect( m_objectVersion, &ObjectVersionFront::objectVersionChanged, this, &PropertyValueModel::refreshPropertyValues );
			QObject::connect( m_objectVersion, &ObjectVersionFront::propertiesForDisplayChanged, this, &PropertyValueModel::refreshPropertyValues );
			QObject::connect( m_objectVersion, &ObjectVersionFront::propertiesChanged, this, &PropertyValueModel::refreshPropertyValues );
		}

		// Refresh.
		this->refreshPropertyValues();
	}
	this->endResetModel();

	// The object version was changed.
	emit objectVersionChanged();
}

//! Returns data for display.
void PropertyValueModel::forDisplay( const QModelIndex & index, QVariant& variant ) const
{
	QJsonValue asValue = m_propertyValues.at( index.row() );
	variant.setValue( asValue.toObject()[ "TypedValue" ].toObject()[ "DisplayeValue" ].toString() );
}

//! Returns data for decoration.
void PropertyValueModel::forDecoration( const QModelIndex & index, QVariant& variant ) const
{
	qDebug( "Decoration role" );
}

//! Returns data for property definition id role.
void PropertyValueModel::forPropertyDefinitionId( const QModelIndex & index, QVariant& variant ) const
{
	QJsonValue asValue = m_propertyValues.at( index.row() );
	double id = asValue.toObject()[ "PropertyDef" ].toDouble();
	variant.setValue( id );
}

//! Returns data for property value role.
void PropertyValueModel::forPropertyValue( const QModelIndex & index, QVariant& variant ) const
{
	// For the property value role, we can return the data as-is.
	variant.setValue( m_propertyValues.at( index.row() ) );
}

//! Refreshes the property values based on the current filter and object version.
void PropertyValueModel::refreshPropertyValues()
{
	// Skip if object version is still unavailable.
	if( m_objectVersion == 0 )
		return;

	// Select the values we want to show based on the filter and then
	// fetch the appropriate values from the object version.
	switch ( m_filter )
	{
	case PropertyValuesForDisplay:
		m_propertyValues = m_objectVersion->propertiesForDisplay().toArray();
		break;

	// Unexpected filter.
	default:
		qCritical( "TODO: Report error" );
		break;
	}
}

