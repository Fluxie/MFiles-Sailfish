/*
 *  Copyright 2013 Juha Lepola
 *
 *  This file is part of M-Files for Sailfish.
 *
 *  M-Files for Sailfish is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  M-Files for Sailfish is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with M-Files for Sailfish. If not, see
 *  <http://www.gnu.org/licenses/>.
 */

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
