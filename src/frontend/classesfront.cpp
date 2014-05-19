#include "classesfront.h"

#include "../backend/asyncfetch.h"
#include "../backend/classcache.h"
#include "../mfiles/mfilesconstants.h"
#include "../mfiles/valuelistitem.h"
#include "../backend/vaultcore.h"

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
AsyncFetch* ClassesFront::items()
{
	// Fetch items from the base class.
	AsyncFetch* items = ValueListFront::items();

	if( MFiles::Constants::AllObjectTypes != m_objectType	)
	{
		// Set the filter.
		items->appendFilter( [=]( const QJsonValue& input ) -> bool {

			// Accept only classes of a specific object type.
			MFiles::ValueListItem item( input );
			return m_classes.contains( item.id() );

		} );
	}
	else
	{
		// No need to apply a separate filter.

	}  // end if.

	// Return the items.
	return items;
}

//! Refreshes the available classes from the vault.
void ClassesFront::refreshClasses()
{
	// Get the classes of the object type we represent.
	if( MFiles::Constants::AllObjectTypes != m_objectType	)
	{
		// Collect the classes of the specified object type.
		m_classes.clear();
		foreach( int classId, this->vault()->classes()->classesOfObjectType( m_objectType ) )
		{
			m_classes.insert( classId );
		}
	}
	else
	{
		// Collect all classes.
		auto items = this->vault()->classes()->list();
		QObject::connect( items, &AsyncFetch::finished, [=]() mutable {
			items->deleteLater();

			m_classes.clear();
			foreach( MFiles::ValueListItem cls, items->values() )
			{
				m_classes.insert( cls.id() );
			}

		} );

	}  // end if
}
