#include "allowedlookupsresolver.h"

#include "asyncfetch.h"
#include "mfiles/propertydef.h"
#include "mfiles/propertyvalue.h"
#include "mfilesconstants.h"
#include "propertyvaluemodel.h"
#include "typedvaluefilter.h"
#include "valuelistfront.h"
#include "vaultfront.h"

AllowedLookupsResolver::AllowedLookupsResolver( PropertyValueModel* parent, VaultFront* vault  ) :
	QObject( parent ),
	m_model( parent ),
	m_vault( vault )
{
	// Connect signals.
}

void AllowedLookupsResolver::requestValueResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles )
{
	Q_ASSERT( topLeft.row() == bottomRight.row() );

	// We need to attempt to resolve the ownership information only if the property value was changed.
	bool filterChanged = roles.size() == 0;
	foreach( int role, roles )
	{
		if( role == PropertyValueModel::FilterRole )
			filterChanged = true;
	}
	if( ! filterChanged )
		return;  // Other than changes to the property value do not affect ownership information.

	// Fetch the current value and abort resolution if the property isn't lookup.
	PropertyValue currentValue( m_model->data( topLeft, PropertyValueModel::PropertyValueRole ).toJsonValue() );
	if( currentValue.typedValue().dataType() != MFilesConstants::MultiSelectLookup ||
		currentValue.typedValue().dataType() != MFilesConstants::SingleSelectLookup  )
		return;

	// Fetch the allowed values using the new filter.
	PropertyDef propertyDef( m_vault->get( VaultFront::PropertyDefinition, currentValue.propertyDef() ) );
	Q_ASSERT( propertyDef.basedOnValueList() );
	TypedValueFilter* filter = qvariant_cast< TypedValueFilter* >( m_model->data( topLeft, PropertyValueModel::FilterRole ) );
	filter->deleteLater();
	ValueListFront* valueList = m_vault->valueList( propertyDef.valueList(), filter );
	AsyncFetch* fetchAvailable = valueList->availableItems( currentValue.typedValue().getLookupIds() );
	QObject::connect( fetchAvailable, &AsyncFetch::finished,  [=]() {

		// Check which of the current values are allowed and update the value if necessary.
		QJsonArray allowedValues = fetchAvailable->values();
		Q_ASSERT( false );

	} );

}
