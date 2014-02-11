#include "propertyvalueownerresolver.h"

#include <QJsonObject>
#include <QList>
#include <QSet>
#include <QMultiHash>
#include <QHash>

#include "asyncfetch.h"
#include "propertyvaluelistmodel.h"
#include "mfiles/objtype.h"
#include "mfiles/propertydef.h"
#include "mfiles/propertyvalue.h"
#include "mfiles/typedvalue.h"
#include "mfiles/valuelistitem.h"
#include "typedvaluefilter.h"
#include "valuelistfront.h"
#include "vaultfront.h"

PropertyValueOwnerResolver::PropertyValueOwnerResolver(
		PropertyValueListModel* parent,
		VaultFront* vault
) :
	QObject( parent ),
	m_model( parent ),
	m_vault( vault ),
	m_ownerLocationVersion( 0 )
{
	// Connect signals.
	QObject::connect( parent, &PropertyValueListModel::dataChanged, this, &PropertyValueOwnerResolver::requestOwnerResolution );
	QObject::connect( parent, &PropertyValueListModel::dataChanged, this, &PropertyValueOwnerResolver::requestOwnershipInfoResolution );
	QObject::connect( parent, &PropertyValueListModel::modelReset, this, &PropertyValueOwnerResolver::refreshOwnershipInfo );
	QObject::connect( this, &PropertyValueOwnerResolver::ownerDetermined, parent, &PropertyValueListModel::suggestData );
	QObject::connect( this, &PropertyValueOwnerResolver::ownershipInfoChanged, parent, &PropertyValueListModel::dataChanged );
}

//! Requests the owner of the specified item.
QJsonValue PropertyValueOwnerResolver::ownershipInfo( const QModelIndex& index ) const
{
	qDebug( "PropertyValueOwnerResolver::ownershipInfo" );

	// Returns the owner of the current item as a JSON value.
	// This owner can be a collection of values of multiple property values as items from one
	// owner value list can be exposed via multiple property definitions.

	// Collect lookup values of all owners and combine them.
	QList< int > ownerPropertyLocations = m_ownerLocationsBySubListLocation.values( index.row() );
	QJsonArray ownerLookupsAll;
	foreach( int ownerPropertyLocation, ownerPropertyLocations )
	{
		// Collect all lookups from this owner.		
		QModelIndex ownerIndex = m_model->index( ownerPropertyLocation );		
		QVariant ownerData = m_model->data( ownerIndex, PropertyValueListModel::PropertyValueRole );
		PropertyValue owner( ownerData.toJsonValue() );
		QJsonArray ownerLookups = owner.typedValue().asLookups();
		foreach( Lookup lookup, ownerLookups )
		{			
			ownerLookupsAll.push_back( lookup.value() );

		}  // end foreach		

	}  // end foreach

	// Construct and return a typed value based on the owner lookups.
	TypedValue ownerValue( ownerLookupsAll );
	return ownerValue.value();
}

bool PropertyValueOwnerResolver::mayHaveOwner( const QModelIndex& subItem ) const
{
	return m_ownerLocationsBySubListLocation.find( subItem.row() ) != m_ownerLocationsBySubListLocation.constEnd();
}

//! Data in the model has changed and a resolution of ownership is required.
void PropertyValueOwnerResolver::requestOwnerResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles )
{
	qDebug( "Owner resolution requested." );

	// The resolution can be made only for one item at the time.
	Q_ASSERT( topLeft.row() == bottomRight.row() );

	// We need to attempt to resolve the owner only if the property value was changed.
	bool propertyValueChanged = roles.size() == 0;
	foreach( int role, roles )
	{
		if( role == PropertyValueListModel::PropertyValueRole )
			propertyValueChanged = true;
	}
	if( ! propertyValueChanged )
		return;  // Other than changes to the property value do not affect owners.

	// Get the new value of the data.
	QVariant changed = m_model->data( topLeft, PropertyValueListModel::PropertyValueRole );
	PropertyValue propertyValue( changed.toJsonValue() );


	// Read the lookup values of the changed item.
	// It will be reui
	TypedValue typedValue = propertyValue.typedValue();
	int dataType = typedValue.dataType();
	QSet< int > ids;
	switch( dataType )
	{
	// Single-select lookup.
	case 9:
	{
		if( typedValue.hasValue() )
		{
			Lookup singleLookup = typedValue.lookup();
			ids.insert( singleLookup.item() );
		}
		break;
	}

	// Multi-select lookup.
	case 10:
	{
		// Collect the ids of multi-select lookups.
		if( typedValue.hasValue() )
		{
			QJsonArray lookups = typedValue[ "Lookups" ].toArray();
			foreach( QJsonValue lookupValue, lookups )
			{
				Lookup lookup( lookupValue );
				ids.insert( lookup.item() );
			}
		}
		break;
	}

	// Other data types do not require owner resolution.
	default:
		return;
	};
	if( ids.size() != 1 )
		return;  // Cannot resolve anything with lookup count other than 1.

	// Get the property definition describin this property value.
	int propertyDefId = propertyValue.propertyDef();
	QJsonValue propertyDef = m_vault->get( VaultFront::PropertyDefinition, propertyDefId );
	Q_ASSERT( propertyDef.toObject()[ "BasedOnValueList" ].toBool() );
	int valueListId = propertyDef.toObject()[ "ValueList" ].toDouble();
	TypedValueFilter* filter = qvariant_cast< TypedValueFilter* >( m_model->data( topLeft, PropertyValueListModel::FilterRole ) );
	filter->deleteLater();
	ValueListFront* valueList = m_vault->valueList( valueListId, filter );	
	AsyncFetch* fetchSubItem = 0;
	if( ids.size() == 1 )
		fetchSubItem = valueList->item( *ids.begin() );

	// Determine the owners we should automatically fill.
	// We will attempt the filling only if the owner is available.
	int row = topLeft.row();
	if( fetchSubItem )
	{
		// Process the owners after the value list item becomes available.
		int ownerLocationVersion = m_ownerLocationVersion;
		QObject::connect( fetchSubItem, &AsyncFetch::finished,  [=]() {

			// We are doing the owner location processing asynchronously.
			// Ensure that the owner location information has not been changed.
			if( m_ownerLocationVersion != ownerLocationVersion )
				return;

			QList< int > ownerPropertyLocations = m_ownerLocationsBySubListLocation.values( row );
			ValueListItem valueListItem( fetchSubItem->value() );
			int ownerId = valueListItem.ownerId();
			foreach( int ownerPropertyLocation, ownerPropertyLocations )
			{
				qDebug( "Processing owner location." );

				// Try auto-filling the owner location.
				tryAutoFillOwnerLocation( ownerLocationVersion, ownerPropertyLocation, ownerId );

			}  // end foreach

		} ); // end connect

	}  // end foreach.

	qDebug( "Owner resolution finished." );
}

/**
 * @brief requestOwnershipInfoResolution
 * @param topLeft
 * @param bottomRight
 */
void PropertyValueOwnerResolver::requestOwnershipInfoResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector< int >& roles )
{
	Q_ASSERT( topLeft.row() == bottomRight.row() );

	// We need to attempt to resolve the ownership information only if the property value was changed.
	bool propertyValueChanged = roles.size() == 0;
	foreach( int role, roles )
	{
		if( role == PropertyValueListModel::PropertyValueRole )
			propertyValueChanged = true;
	}
	if( ! propertyValueChanged )
		return;  // Other than changes to the property value do not affect ownership information.

	// The ownership information of each sub-item has potentially changed when the owner is updated.
	// Check if the owner value has totally changed forcing us to clear the value of the subitem
	// or if it is sufficient to just update the typed value filter.
	QList< int > subItems = m_subItemLocationByOwnerLocation.values( topLeft.row() );
	QVector< int > modifiedRoles;
	modifiedRoles.push_back( PropertyValueListModel::FilterRole );
	foreach( int subItemRow, subItems )
	{
		// Emit the change.
		qDebug("Subitem changed");
		QModelIndex subItemIndex = m_model->index( subItemRow );
		emit ownershipInfoChanged( subItemIndex, subItemIndex, modifiedRoles );

	}  // end foreach
}

void PropertyValueOwnerResolver::refreshOwnershipInfo()
{
	qDebug( "Refreshing ownership info." );

	// Loop through the items in the model and find properyt values with owners.
	QMultiHash< int, int > rowsOfValueList;  // Tracks the position of property values based on
	QHash< int, int > ownersOfValueLists;  // Tracks the value list ownership.
	QHash< int, int > propertyValuesByRow;  // Tracks the rows of property values.
	QHash< int, QJsonValue > propertyDefsByRow;  // Tracks property definitions.
	for( int row = 0; row < m_model->rowCount(); row++ )
	{
		// Check if the
		PropertyValue propertyValue( m_model->data( m_model->index( row ), PropertyValueListModel::PropertyValueRole ).toJsonValue() );
		propertyValuesByRow.insert( propertyValue.propertyDef(), row );  // Store the location of the property definition.
		int dataType = propertyValue.typedValue().dataType();
		if( dataType == 9  || dataType == 10 )
		{
			// This property value is based on a value list.
			// Does this value list have an owner?
			PropertyDef propertyDef( m_vault->get( VaultFront::PropertyDefinition, propertyValue.propertyDef() ) );
			Q_ASSERT( propertyDef.basedOnValueList() );
			propertyDefsByRow.insert( row, propertyDef.value() );
			rowsOfValueList.insert( propertyDef.valueList(), row );  // Store thw row where this value list is located.
			ObjType objtype( m_vault->get( VaultFront::ObjectType, propertyDef.valueList() ) );
			if( objtype.hasOwner() )
			{
				// Store thw owner information
				qDebug( "Mapping ownership." );
				ownersOfValueLists.insert( propertyDef.valueList(), objtype.owner() );
			}

		}  // end if

	}  // end for

	// Construct the actual owener mapping that we can use when we try to detect the owners.
	m_ownerLocationsBySubListLocation.clear();
	m_ownerLocationVersion++;
	for( QHash< int, int >::const_iterator itr = ownersOfValueLists.begin(); itr != ownersOfValueLists.end(); itr++ )
	{
		// Get the owner list.
		int sublist = itr.key();
		int owner = itr.value();
		qDebug( QString( "Processing sublist %1 and owner %2." ).arg( sublist ).arg( owner ).toLatin1() );

		// TODO: Karteesinen tulo.
		QList< int > subListLocations = rowsOfValueList.values( sublist );
		QList< int > ownerLocations = rowsOfValueList.values( owner );
		qSort( ownerLocations );  // Sort the owners for furher processing.
		foreach( int subListLocation, subListLocations )
		{
			// Skip processing this sublist-owner combination if no owners are present.
			if( ownerLocations.isEmpty() )
				break;

			// The owner value of the current property value is the one that is the next compatible owner when moving upwards in
			// the property value listing starting from the current property value.
			// We track this location by starting iterating from the first owner until we reach the position of the
			// owner that matches the description above.
			PropertyDef subItemPropertyDef( propertyDefsByRow.value( subListLocation ) );
			Q_ASSERT( subItemPropertyDef.basedOnValueList() );
			QList< int >::const_iterator ownerCandidateLocation = ownerLocations.constBegin();
			for( QList< int >::const_iterator ownerCandidateSeeker = ownerLocations.constBegin();
				ownerCandidateSeeker != ownerLocations.end(); ownerCandidateSeeker++ )
			{
				// Break the search after we have found the first valid owner that is
				// after the subitem in the property value listing.
				PropertyDef ownerCandidatePropertyDef( propertyDefsByRow.value( *ownerCandidateSeeker ) );
				Q_ASSERT( ownerCandidatePropertyDef.basedOnValueList() );
				if( (*ownerCandidateSeeker) > subListLocation &&
					ownerCandidatePropertyDef.isValidOwnerFor( subItemPropertyDef ) )
					break;

				// The current owner candidate seeker is a valid owner? Then store it as a next candidate.
				if( ownerCandidatePropertyDef.isValidOwnerFor( subItemPropertyDef ) )
					ownerCandidateLocation = ownerCandidateSeeker;
			}  // end if

			// Check if we ended up at the correct position.
			PropertyDef ownerCandidatePropertyDef( propertyDefsByRow.value( *ownerCandidateLocation ) );
			Q_ASSERT( ownerCandidatePropertyDef.basedOnValueList() );
			if( ownerCandidatePropertyDef.isValidOwnerFor( subItemPropertyDef ) )
			{
				qDebug( "Valid owner found." );
				int ownerLocation = (*ownerCandidateLocation);
				m_ownerLocationsBySubListLocation.insert( subListLocation, ownerLocation );
				m_subItemLocationByOwnerLocation.insert( ownerLocation, subListLocation );

			}  // end if

		}  // end foreach

	}  // end foreach.
}

void PropertyValueOwnerResolver::tryAutoFillOwnerLocation( int ownerLocationVersion, int ownerPropertyLocation, int ownerId )
{
	// Resolve the possible owner value and try automatically filling the owner
	// if it has not been set.
	QModelIndex ownerIndex = m_model->index( ownerPropertyLocation );
	QVariant owner = m_model->data( ownerIndex, PropertyValueListModel::PropertyValueRole );
	PropertyValue ownerValue( owner.toJsonValue() );
	if( ! ownerValue.hasValue() )
	{
		// The owner is not specified, we can select it automatically based on the selected value.

		// First we get the item id of the owner which can be obtained by fetching the value list item of
		PropertyDef ownerPropertyDef = m_vault->get( VaultFront::PropertyDefinition, ownerValue.propertyDef() );
		Q_ASSERT( ownerPropertyDef.basedOnValueList() );
		TypedValueFilter* ownerFilter = qvariant_cast< TypedValueFilter* >( m_model->data( ownerIndex, PropertyValueListModel::FilterRole ) );
		ownerFilter->deleteLater();
		ValueListFront* ownerValueList = m_vault->valueList( ownerPropertyDef.valueList(), ownerFilter );
		AsyncFetch* fetchOwner = ownerValueList->item( ownerId );
		int propertyDef = ownerValue.propertyDef();
		QObject::connect( fetchOwner, &AsyncFetch::finished,  [=]() {

			// We are doing the auto-filling of the owner location asynchronously.
			// Ensure that the owner location information has not been changed.
			if( m_ownerLocationVersion != ownerLocationVersion )
				return;

			// Get the owner value.
			ValueListItem newOwnerAsItem = ValueListItem( fetchOwner->value() );

			// Construct lookup based on the owner item and set it to the owner value.
			Lookup newOwnerAsLookup( newOwnerAsItem.toLookup() );
			Q_ASSERT( ! newOwnerAsLookup.displayValue().isEmpty() );
			TypedValue newOwnerAsTypedValue( ownerPropertyDef.dataType(), newOwnerAsLookup );
			PropertyValue newOwner( propertyDef, newOwnerAsTypedValue );

			// Signal that the owner should be changed.
			Q_ASSERT( newOwner.typedValue().hasValue() );
			emit ownerDetermined( ownerIndex, newOwner.value() );

			// Mark the fetch for deletion.
			fetchOwner->deleteLater();

		} );  // end connect

	}  // end if

}
