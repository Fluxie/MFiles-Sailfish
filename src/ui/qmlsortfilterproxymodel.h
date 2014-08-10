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

#ifndef QMLSORTFILTERPROXYMODEL_H
#define QMLSORTFILTERPROXYMODEL_H

#include <QJSValue>
#include <QSortFilterProxyModel>

// Forward declaration.
class DataAccessorSource;

class QmlSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	Q_PROPERTY( QJSValue lessThanJS READ lessThanJS NOTIFY lessThanJSChanged WRITE setLessThanJS )
public:
	explicit QmlSortFilterProxyModel(QObject *parent = 0);

	/**
	 * @brief sortByDefaults Sorts the list using the defaults.
	 */
	Q_INVOKABLE void sortByDefaults();

	/**
	 * @brief Gets the function that performs the comparsion in JavaScript.
	 * @return The function that determines the value of the custom role.
	 */
	QJSValue lessThanJS() const { return m_lessThanJS; }

signals:

	/**
	 * @brief lessThanJSChanged is signaled when the value of lessThanJS has changed.
	 */
	void lessThanJSChanged();

public slots:

	/**
	 * @brief Sets the JavaScript function for comparing two values.
	 * @param lessThanFunction The comparison function.
	 */
	void setLessThanJS( const QJSValue& lessThanFunction );

// QSortFilterProxyModel
protected:

	/**
	 * @brief Override for lessThan
	 * @param left
	 * @param right
	 * @return Returns true if the value of the item referred to by the given index left is less than the value of the item referred to by the given index right, otherwise returns false.
	 */
	virtual bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

private slots:

	/**
	 * @brief Updates the data accessor source from the current sourceModel->
	 */
	void updateDataAccessorSource();

private:

	QJSValue m_lessThanJS;

	DataAccessorSource* m_dataAccessor;

};

#endif // QMLSORTFILTERPROXYMODEL_H
