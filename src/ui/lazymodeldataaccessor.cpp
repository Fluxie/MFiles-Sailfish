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

#include "lazymodeldataaccessor.h"

LazyModelDataAccessor::LazyModelDataAccessor( QAbstractItemModel* model, const QModelIndex& index ) :
	QObject( nullptr ),
	m_model( model ),
	m_index( index )
{
	Q_CHECK_PTR( m_model );
}

QVariant LazyModelDataAccessor::valueOf( int role ) const
{
	return m_model->data( m_index, role );
}
