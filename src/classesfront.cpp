#include "classesfront.h"

#include "classcache.h"
#include "mfilesconstants.h"
#include "vaultcore.h"

ClassesFront::ClassesFront(
	VaultCore* vault,  //!< Vault.
	ValueListCore* core,  //!< Core
	int objectType  //!< The object type used to filter classes.
) :
	ValueListFront( vault, core ),
	m_objectType( objectType )
{
	// Connect signals.
	// We want to automatically refresh the classes.
	QObject::connect( this, &ClassesFront::objectTypeChanged, this, &ClassesFront::refreshClasses );
	QObject::connect( this->vault()->classes(), &ClassCache::refreshed, this, &ClassesFront::refreshClasses );

	// Try refreshing the classes now as they are most likely cached already.
	this->refreshClasses();
}

//! Value list items.
QJsonArray ClassesFront::items()
{
	// Fetch items from the base class.
	QJsonArray items = ValueListFront::items();
	QJsonArray acceptedItems;
	if( MFilesConstants::AllObjectTypes != m_objectType	)
	{
		// Fetching classes of a specific object type.
		// Accept only them.
		foreach( QJsonValue item, items )
		{
			// Include the classes that are meant for our object type in the accepted items.
			QJsonObject asObject = item.toObject();
			int id = asObject[ "ID" ].toDouble();
			if( m_classes.contains( id ) )
				acceptedItems.push_back( item );
		}
	}
	else
	{
		// Fetching classes of all object types.
		acceptedItems = items;

	}  // end if.

	// Return the accepted items.
	return acceptedItems;
}

//! Refreshes the available classes from the vault.
void ClassesFront::refreshClasses()
{
	// Get the classes of the object type we represent.
	if( MFilesConstants::AllObjectTypes != m_objectType	)
	{
		// Collect the classes of the specified object type.
		m_classes.clear();
		foreach( int classId, this->vault()->classes()->classesOfObjectType( m_objectType ) )
		{
			m_classes.insert( classId );
		}
	}
	else
		m_classes.clear();
}
