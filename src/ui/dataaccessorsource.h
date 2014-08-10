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

#ifndef DATAACCESSORSOURCE_H
#define DATAACCESSORSOURCE_H

#include <QModelIndex>

#include "lazymodeldataaccessor.h"

class DataAccessorSource
{
public:

	/**
	 * @brief Initializes new DataAccessorSource.
	 */
	DataAccessorSource() {}

	/**
	 * @brief Destructor.
	 */
	virtual ~DataAccessorSource() {}

	/**
	 * @brief Creates new data accessor.
	 * @return New data accessor
	 */
	virtual LazyModelDataAccessor* createAccessor( const QModelIndex& index ) = 0;

};

#endif // DATAACCESSORSOURCE_H