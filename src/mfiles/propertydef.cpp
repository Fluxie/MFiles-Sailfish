/*
 *  Copyright 2014 Juha Lepola
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

#include "propertydef.h"
#include "ownerproperty.h"
#include "../mfilesconstants.h"

PropertyDef::PropertyDef( const QJsonValue& propertyDef ) :
	MFilesTypeCapsule( propertyDef )
{
}

bool PropertyDef::isValidOwnerFor( const PropertyDef& subItemRepresentative ) const
{
	// Sanity.
	Q_ASSERT( this->basedOnValueList() );
	Q_ASSERT( subItemRepresentative.basedOnValueList() );

	// This function does not check if this property definition represents a value list that
	// actually is the owner value list of the value list that subItemRepresentative represents.
	// We only check that if the filtering settings specified in subItemRepresentative make
	// us a potential owner fo the subItemRepresentative property definition.
	OwnerProperty subOwnerProperty( subItemRepresentative.ownerPropertyDef() );
	if( subOwnerProperty.id() == MFilesConstants::AutomaticFiltering ||
		subOwnerProperty.id() == this->id() )
	{
		// Automatic filtering was specified or we are the designated owner property.
		// Either way, we are a valid owner.
		qDebug( "isValidOwnerFor");
		return true;
	}
	else
	{
		// We are not a valid owner.
		qDebug( "isValidOwnerFor");
		return false;
	}
}
