#include "valuelistcore.h"

#include "vaultcore.h"

ValueListCore::ValueListCore(VaultCore* vault, int valueList ) :
	StructureCacheBase( QString( "/valuelists/%1/items?page" ).arg( valueList ), vault, false ),
	m_valueList( valueList ),
	m_propertyDefinition( -1 )
{
}

//! Constructs value list object to represent one value list accessed via the specified property definition.
ValueListCore::ValueListCore( VaultCore* vault, int valueList, int propertyDefinition ) :
	StructureCacheBase( QString( "/valuelists/%1/items?page&propertydef=%2" ).arg( valueList ).arg( propertyDefinition ), vault, true ),
	m_valueList( valueList ),
	m_propertyDefinition( propertyDefinition )
{

}
