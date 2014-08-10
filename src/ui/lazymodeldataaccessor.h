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

#ifndef LAZYMODELDATAACCESSOR_H
#define LAZYMODELDATAACCESSOR_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <QVariant>

/**
 * @brief The LazyModelDataAccessor class is an accessor to one
 */
class LazyModelDataAccessor : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QVariant display READ display )
public:

	/**
	 * @brief Initializes new LazyModelDataAccessor object.
	 * @param parent
	 */
	explicit LazyModelDataAccessor( QAbstractItemModel* model, const QModelIndex& index );

	/**
	 * @brief display
	 * @return The valeu of the display role.
	 */
	QVariant display() const { return this->valueOf(  Qt::DisplayRole ); }

signals:

public slots:

protected:

	/**
	 * @brief Gets the value of the given role.
	 * @param role Identifies the value
	 * @return The value of the given role.
	 */
	QVariant valueOf( int role ) const;

private:

	QAbstractItemModel* m_model;
	QModelIndex m_index;

};

#endif // LAZYMODELDATAACCESSOR_H
