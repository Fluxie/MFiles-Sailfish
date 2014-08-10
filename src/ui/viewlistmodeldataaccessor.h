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

#ifndef VIEWLISTMODELDATAACCESSOR_H
#define VIEWLISTMODELDATAACCESSOR_H

#include "lazymodeldataaccessor.h"
#include "viewlistmodel.h"

/**
 * @brief The ViewListModelDataAccessor class is a lazy accessor class for accessing the data of the view list model.
 */
class ViewListModelDataAccessor : public LazyModelDataAccessor
{
	Q_OBJECT
	Q_PROPERTY( QVariant resource READ resource )
	Q_PROPERTY( QVariant data READ data )
	Q_PROPERTY( QVariant type READ type )
public:

	/**
	 * @brief Initializes new ViewListModelDataAccessor object.
	 * @param model The moel
	 * @param index Index of the data accessed.
	 */
	explicit ViewListModelDataAccessor( ViewListModel* model, const QModelIndex& index );

	/**
	 * @brief resource
	 * @return The value of the resource role.
	 */
	QVariant resource() const { return this->valueOf( ViewListModel::ResourceRole ); }

	/**
	 * @brief data
	 * @return The value of the data role.
	 */
	QVariant data() const { return this->valueOf( ViewListModel::DataRole ); }

	/**
	 * @brief data
	 * @return The value of the type role.
	 */
	QVariant type() const { return this->valueOf( ViewListModel::TypeRole ); }

signals:

public slots:

};

#endif // VIEWLISTMODELDATAACCESSOR_H
