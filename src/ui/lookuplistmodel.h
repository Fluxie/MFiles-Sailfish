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

#ifndef LOOKUPListMODEL_H
#define LOOKUPListMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QJsonArray>
#include <QJsonValue>

class LookupListModel : public QAbstractListModel
{
	//! The role id of the lookup role.
	static const int LookupRole;

	Q_OBJECT
	Q_PROPERTY( int rowLimit READ rowLimit WRITE setRowLimit NOTIFY rowLimitChanged )
	Q_PROPERTY( QJsonValue propertyValue READ propertyValue WRITE setPropertyValue NOTIFY propertyValueChanged )
	Q_PROPERTY( int lookupCount READ lookupCount NOTIFY lookupCountChanged )
public:

	//! Initializes the lookup model.
	explicit LookupListModel(QObject *parent = 0);

	//! Maximum number of lookups to display.
	int rowLimit() const { return m_rowLimit; }

	//! Property value.
	QJsonValue propertyValue() const { return m_propertyValue; }

	//! Lookup.
	Q_INVOKABLE QJsonValue lookup( int row ) const { return m_lookups[ row ]; }

	//! The actual number of lookups within this model.
	int lookupCount() const { return m_lookups.size(); }

// Interface implementing the model.
public:

	//! Returns the number of rows under the given parent.
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

	//! Returns the data stored under the given role for the item referred to by the index.
	virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

	//! Role names. Note: The documentation claims that we should call setRoleNames to specify the roles. However, this function no longer exists and roleNAmes has been made virtula.
	virtual QHash< int,QByteArray > roleNames() const;

	//! Flags.
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	//! Sets the data.
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

signals:

	//! Signaled when the row limit changes.
	void rowLimitChanged();

	//! Signaled when the property value changes.
	void propertyValueChanged();

	//! Signaled when the number of lookups changes.
	void lookupCountChanged();

public slots:

	//! Sets the maximum number of rows to display.
	void setRowLimit( int rowLimit );

	//! Sets property value.
	void setPropertyValue( const QJsonValue propertyValue );

// Private interface.
private:

	//! Updates the property value from the current lookups.
	void updatePropertyValueFromLookups();

	//! Returns data for display.
	void forDisplay( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for decoration.
	void forDecoration( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for lookup.
	void forLookup( const QModelIndex & index, QVariant& variant ) const;

// Private data.
private:

	int m_rowLimit;  //!< The maximum number of rows to display.
	QJsonValue m_propertyValue;  //!< The property value.
	int m_dataType;  //!< The datatype of the typed value inside the property value.
	QJsonArray m_lookups;  //!< Lookups extrated from the property value for convenience.

};

#endif // LOOKUPListMODEL_H
