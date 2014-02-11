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

#include "propertyvaluemodel.h"

#include <QHash>
#include <QVariantMap>

#include "allowedlookupsresolver.h"
#include "objectversionfront.h"
#include "objver.h"
#include "propertyvalueownerresolver.h"
#include "typedvaluefilter.h"
#include "mfiles/propertyvalue.h"
#include "mfiles/typedvalue.h"


//! The role id of the property definition id role.
const int PropertyValueModel::PropertyDefinitionIdRole = Qt::UserRole;

//! The role id the property value role.
const int PropertyValueModel::PropertyValueRole = Qt::UserRole + 1;

//! The role id the filter role.
const int PropertyValueModel::FilterRole = Qt::UserRole + 2;

PropertyValueModel::PropertyValueModel(QObject *parent) :
	QAbstractListModel(parent),
	m_filter( Undefined ),
	m_objectVersion( 0 ),
	m_ownerResolver( 0 ),
	m_vault( 0 )
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

	// Filter.
	case PropertyValueModel::FilterRole :
		this->forFilter( index, data );
		break;

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
	}
	return data;
}

//! Flags.
Qt::ItemFlags PropertyValueModel::flags( const QModelIndex &index ) const
{
	Q_ASSERT( index.isValid() );

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

	// Read the input value to QJsonValue if possible.
	QJsonValue newValue;
	if( value.type() == QVariant::Map )
	{
		// Variant map.
		newValue = QJsonObject::fromVariantMap( qvariant_cast< QVariantMap >( value ) );
	}
	else if( value.type() == QVariant::nameToType( "QJsonValue" ) )
	{
		// QJSonValue.
		newValue = qvariant_cast< QJsonValue >( value );
	}
	else
	{
		// TODO: Report error.
		Q_ASSERT( false );
		qCritical( value.typeName() );
		return false;
	}

	// Check if the value has changed.
	const QJsonValue& previousValue = m_propertyValues[ index.row() ];
	if( previousValue == newValue )
	{
		qDebug( "Value was not updated." );
		return false;
	}

	// The value denoted by the index has changed. Update it with the new value and signal the change.
	PropertyValue asPropertyValue( newValue );
	qDebug( QString( "Update property value, Has value %1" ).arg( asPropertyValue.typedValue().hasValue() ).toLatin1() );
	m_propertyValues[ index.row() ] = newValue;
	QVector< int > changedRoles;
	changedRoles.push_back( PropertyValueModel::PropertyValueRole );
	changedRoles.push_back( Qt::DisplayRole );
	QModelIndex refreshedIndex = this->index( index.row() );
	emit dataChanged( refreshedIndex, refreshedIndex, changedRoles );
	return true;
}

/**
 * @brief Submit the changes made to the model.
 * @return True if the changes we succesfully submitted.
 */
bool PropertyValueModel::submit()
{
	qDebug( "Submit called." );

	// Skip.
	if( m_objectVersion == 0 )
		return false;

	// Send the properties to the server.
	m_objectVersion->sendPropertiesToServer( m_propertyValues );

	// Succeeded.
	return true;
}

/**
 * @brief Revert the changes made to the model.
 */
void PropertyValueModel::revert()
{
	qDebug( "Revert called." );

	// Reload the property values.
	this->refreshPropertyValues();
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
	roles.insert( PropertyValueModel::FilterRole, QString( "filter" ).toLatin1() );
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
		this->refreshPropertyValuesImpl();
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
		this->refreshPropertyValuesImpl();
	}
	this->endResetModel();

	// The object version was changed.
	emit objectVersionChanged();
}

void PropertyValueModel::setVault( VaultFront* vault )
{
	// Skip if identical.
	if( m_vault == vault )
		return;

	// Change the vault.
	this->beginResetModel();
	{
		m_vault = vault;
		this->refreshPropertyValuesImpl();
	}
	this->endResetModel();
	emit vaultChanged();
}

/**
 * @brief suggestData
 * @param index The location of the new data.
 * @param propertyValue The new value for the location.
 */
void PropertyValueModel::suggestData( const QModelIndex& index, const QJsonValue& propertyValue )
{
	Q_ASSERT( index.isValid() );

	// Delegate.
	this->setData( index, QVariant( propertyValue ), PropertyValueModel::PropertyValueRole );
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

//! Returns data for filter role.
void PropertyValueModel::forFilter( const QModelIndex & index, QVariant& variant ) const
{
	// Get property definition id.
	qDebug( "for filter");
	QJsonValue asValue = m_propertyValues.at( index.row() );
	QJsonObject asObject = asValue.toObject();
	int id = asObject[ "PropertyDef" ].toDouble();
	TypedValueFilter* filter = 0;
	if( m_ownerResolver->mayHaveOwner( index ) )
		filter = TypedValueFilter::forPropertyDefinition( id, index, m_ownerResolver );
	else
		filter = TypedValueFilter::forPropertyDefinition( id );
	filter->setObjectType( m_objectVersion->objver().type() );
	variant.setValue( filter );
}


void PropertyValueModel::refreshPropertyValues()
{
	this->beginResetModel();
	{
		this->refreshPropertyValuesImpl();
	}
	this->endResetModel();
}

//! Refreshes the property values based on the current filter and object version.
void PropertyValueModel::refreshPropertyValuesImpl()
{
	// Skip if object version or vault are still unavailable.
	if( m_objectVersion == 0 || m_vault == 0 )
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

	// Establish owner resolved if still unavailable.
	if( m_ownerResolver == 0 )
	{
		m_ownerResolver = new PropertyValueOwnerResolver( this, m_vault );
		m_ownerResolver->refreshOwnershipInfo();
	}
	if( m_allowedLookupsResolvers == 0 )
		m_allowedLookupsResolvers = new AllowedLookupsResolver( this, m_vault );
}

