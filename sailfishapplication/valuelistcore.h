#ifndef VALUELISTCORE_H
#define VALUELISTCORE_H

#include <QHash>
#include <QJsonValue>
#include <QObject>
#include <QVector>

#include "structurecachebase.h"

// Forward declarations.
class VaultCore;

class ValueListCore : public StructureCacheBase
{
	Q_OBJECT
public:

	//! Constructs value list object to represent one value list.
	explicit ValueListCore( VaultCore* vault, int valueList );

	//! Constructs value list object to represent one value list accessed via the specified property definition.
	explicit ValueListCore( VaultCore* vault, int valueList, int propertyDefinition );

	//! The id of the value list.
	int id() const { return m_valueList; }

	//! Property definition the values in this list are filtered with. -1 if not filtered.
	int propertyDefinition() const { return m_propertyDefinition; }

signals:

public slots:

// Private data.
private:

	// Static data that does not need protection.
	int m_valueList;  //!< The id of the value list this object represents.
	int m_propertyDefinition;  //!< The id of the property definition this object represents.	

};

#endif // VALUELISTCORE_H
