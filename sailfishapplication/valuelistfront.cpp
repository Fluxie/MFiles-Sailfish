#include "valuelistfront.h"


#include "objecttypecache.h"
#include "valuelistcore.h"
#include "vaultcore.h"

ValueListFront::ValueListFront(
	VaultCore* vault,  //!< Vault.
	ValueListCore* core  //!< Core
) :
	FrontBase( core ),
	m_id( core->id() ),
	m_propertyDefinition( core->propertyDefinition() ),
	m_vault( vault )
{
	// Connect refresh events.
	QObject::connect( core, &ValueListCore::refreshed, this, &ValueListFront::refreshed );

	// Connect status changed events:
	// - Core has been populated.
	QObject::connect( core, &ValueListCore::populatedChanged, this, &ValueListFront::statusChanged );
}

//! Value list items.
QJsonArray ValueListFront::items()
{
	// Empty?
	if( ! this->core() )
		return QJsonArray();

	// Fetch the core and return the value list.
	qDebug( "Returning value list items...");
	ValueListCore* core = this->valueList();	
	return core->list();
}

//! Status.
ValueListFront::Status ValueListFront::status()
{
	// Disconnected?
	if( this->core() == 0 )
		return ValueListFront::Disconnected;

	// Is the value list empty.
	ValueListCore* core = this->valueList();
	if( ! core->populated() )
		return ValueListFront::Empty;

	// We are in a ready state.
	return ValueListFront::Ready;
}

//! Does this front accept the specified core.
bool ValueListFront::accept( QObject* coreCandidate ) const
{
	// Can we accept the specified core.
	qDebug( "Requesting acceptance for ValueListCore." );
	ValueListCore* core = qobject_cast< ValueListCore* >( coreCandidate );
	if( core != 0 && core->id() == m_id && core->propertyDefinition() == m_propertyDefinition )
		return true;
	else
		return false;
}
