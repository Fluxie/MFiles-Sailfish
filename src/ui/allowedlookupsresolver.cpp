#include "allowedlookupsresolver.h"

#include "../backend/asyncfetch.h"
#include "../mfiles/propertydef.h"
#include "../mfiles/propertyvalue.h"
#include "../mfiles/valuelistitem.h"
#include "../mfiles/mfilesconstants.h"
#include "propertyvaluelistmodel.h"
#include "../backend/typedvaluefilter.h"
#include "../valuelistfront.h"
#include "../vaultfront.h"

AllowedLookupsResolver::AllowedLookupsResolver( PropertyValueListModel* parent, VaultFront* vault  ) :
	QObject( parent ),
	m_model( parent ),
	m_vault( vault )
{
	// Connect signals.
	QObject::connect( parent, &PropertyValueListModel::dataChanged, this, &AllowedLookupsResolver::requestValueResolution );
	QObject::connect( this, &AllowedLookupsResolver::allowedValueResolved, parent, &PropertyValueListModel::suggestData );
}

void AllowedLookupsResolver::requestValueResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles )
{
	Q_ASSERT( topLeft.row() == bottomRight.row() );

	// We need to attempt to resolve the allowed lookups only if the filter was changed.
	bool filterChanged = roles.size() == 0;
	foreach( int role, roles )
	{
		if( role == PropertyValueListModel::FilterRole )
			filterChanged = true;
	}
	if( ! filterChanged )
		return;  // Other than changes to the filter do not affect allowed lookups

	// Fetch the current value and abort resolution if the property isn't lookup.
	MFiles::PropertyValue currentValue( m_model->data( topLeft, PropertyValueListModel::PropertyValueRole ).toJsonValue() );
	if( currentValue.typedValue().dataType() != MFiles::Constants::MultiSelectLookup &&
		currentValue.typedValue().dataType() != MFiles::Constants::SingleSelectLookup  )
		return;

	// Fetch the allowed values using the new filter.
	AsyncFetch* fetchAllowed = this->fetchAllowedItems( topLeft, currentValue.value() );

	// Resolve the validity.
	QObject::connect( fetchAllowed, &AsyncFetch::finished, [=]() mutable {
		fetchAllowed->deleteLater();

		QJsonArray values = fetchAllowed->values();
		this->resolveValidity( values, topLeft, currentValue.value() );
	} );
}

AsyncFetch* AllowedLookupsResolver::fetchAllowedItems( const QModelIndex& currentValueIndex, const QJsonValue& currentValue )
{
	// Get the value list id, contrstruct appropriate typed value filter and then fetch the values.
	MFiles::PropertyValue asPropertyValue( currentValue );
	QSet< int > currentLookupIds = asPropertyValue.typedValue().getLookupIds();
	MFiles::PropertyDef propertyDef( m_vault->get( VaultFront::PropertyDefinition, asPropertyValue.propertyDef() ) );
	Q_ASSERT( propertyDef.basedOnValueList() );
	TypedValueFilter* filter = qvariant_cast< TypedValueFilter* >( m_model->data( currentValueIndex, PropertyValueListModel::FilterRole ) );
	filter->deleteLater();
	ValueListFront* valueList = m_vault->valueList( propertyDef.valueList() , filter );
	AsyncFetch* fetchAvailable = valueList->items();
	fetchAvailable->setParent( this );
	fetchAvailable->appendFilter( [=]( const QJsonValue& input )->bool {

		// Include only current values.
		MFiles::ValueListItem item( input );
		return currentLookupIds.contains( item.id() );

	} );

	return fetchAvailable;
}

void AllowedLookupsResolver::resolveValidity( const QJsonArray& allowedItems, const QModelIndex& index, const QJsonValue& currentValue )
{
	// Convert the allowed value list items to a set.
	QSet< int > allowedItemIds;
	foreach( MFiles::ValueListItem item, allowedItems )
	{
		allowedItemIds.insert( item.id() );
	}

	// Remove all values from the lookup that were not returned
	// and then signal update if something was dropped.
	MFiles::PropertyValue asPropertyValue( currentValue );
	MFiles::TypedValue updatedTypedValue( asPropertyValue.typedValue().value() );
	if( updatedTypedValue.dropLookupsExcept( allowedItemIds ) )
	{
		// Update the value.
		MFiles::PropertyValue updatedValue( asPropertyValue.propertyDef(), updatedTypedValue );
		emit allowedValueResolved( index, updatedValue.value() );
	}
}
