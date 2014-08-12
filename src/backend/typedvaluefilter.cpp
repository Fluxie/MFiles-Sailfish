#include "typedvaluefilter.h"

TypedValueFilter::TypedValueFilter( QObject *parent ) :
	QObject( parent ),
	m_enabled( false ),
	m_objectType( TypedValueFilter::Undefined ),
	m_propertyDef( TypedValueFilter::Undefined ),
	m_ownerInfo( nullptr )
{

}

//! Initializes new typed value filter for property definition.
TypedValueFilter* TypedValueFilter::forPropertyDefinition( int propertyDef )
{
	// Create the filter and return it.
	TypedValueFilter* filter = new TypedValueFilter();
	filter->m_enabled = true;
	filter->m_propertyDef = propertyDef;	
	return filter;
}

//! Initializes new typed value filter for property definition.
TypedValueFilter* TypedValueFilter::forPropertyDefinition( int propertyDef, LazyOwnerInfo::RESOLVER_T ownerInfoResolver )
{
	// Create the filter and return it.
	TypedValueFilter* filter = forPropertyDefinition( propertyDef );
	filter->m_ownerInfo = new LazyOwnerInfo( ownerInfoResolver );
	return filter;
}

QJsonValue TypedValueFilter::ownerInfo() const
{
	// Fetch and return the owner info if it is available.
	if( m_ownerInfo != nullptr )
	{
		// Delegate.
		return m_ownerInfo->ownerInfo();
	}
	else
	{
		// Construct the empty ownership info in similar manner than what is done in PropertyValueOWnerResolver::ownershipInfo.
		QJsonArray noLookups;
		MFiles::TypedValue empty( noLookups );
		return empty.toJsonValue();
	}
}

//! Sets the object type.
void TypedValueFilter::setObjectType( int objectType )
{
	// Skip if the object type was not changed.
	if( m_objectType == objectType )
		return;

	// Change,
	m_objectType = objectType;
	emit objectTypeChanged();
}
