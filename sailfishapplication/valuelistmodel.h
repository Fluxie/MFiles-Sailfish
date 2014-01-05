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

#ifndef VALUELISTMODEL_H
#define VALUELISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>

#include "valuelistfront.h"

//! A model for displaying value list items.
/**
  TODO: Cache the value list. UI control calls the methods quite often.
	This will probably also make it easier to implment partial updates.
 */
class ValueListModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY( ValueListFront* valueList READ valueList WRITE setValueList NOTIFY valueListChanged )
public:

	//! Constructor
	explicit ValueListModel();

// Interface for the model.
public:

	//! Returns the value list.
	ValueListFront* valueList() { return m_valueList; }

	//! Returns the number of rows under the given parent.
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

	//! Returns the data stored under the given role for the item referred to by the index.
	virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

signals:

	//! Signaled when the value list has changed.
	void valueListChanged();

public slots:

	//! Called when the reset of the model data is required.
	void resetFromList();

	//! Sets the value list.
	void setValueList( ValueListFront* valueList );

// Private interface.
private:



	//! Returns data for display.
	QVariant forDisplay( const QModelIndex & index ) const;

	//! Returns data for decoration.
	QVariant forDecoration( const QModelIndex & index ) const;

// Private data.
private:

	ValueListFront* m_valueList;  //!< The value list used to populate the model.
	QJsonArray m_data;  //!< A cached copy of the value list item data.

};

#endif // VALUELISTMODEL_H
