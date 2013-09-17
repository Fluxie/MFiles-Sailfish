#ifndef PROPERTYDEFCACHE_H
#define PROPERTYDEFCACHE_H

#include "structurecachebase.h"

// Forward declarations.
class VaultCore;

class PropertyDefCache : public StructureCacheBase
{
	Q_OBJECT
public:
	explicit PropertyDefCache(
		VaultCore* parent  //!< Parent vault.
	);
	
signals:
	
public slots:
	
};

#endif // PROPERTYDEFCACHE_H
