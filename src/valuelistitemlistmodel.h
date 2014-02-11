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

#ifndef VALUELISTItemListMODEL_H
#define VALUELISTItemListMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QPointer>
#include <QVariant>

#include "backend/typedvaluefilter.h"
#include "valuelistfront.h"

// Forward declarations.
class AsyncFetch;

//! A model for displaying value list items.
class ValueListItemListModel : public QAbstractListModel
{
	//! The role id the lookup role.
	static const int LookupRole;

	//! The role id the id role.
	static const int IdRole;

	Q_OBJECT
	Q_PROPERTY( ValueListFront* valueList READ valueList WRITE setValueList NOTIFY valueListChanged )
	Q_PROPERTY( QJsonValue selectedLookup READ selectedLookup WRITE setSelectedLookup NOTIFY selectedLookupChanged )
	Q_PROPERTY( QJsonArray blockedLookups READ blockedLookups WRITE setBlockedLookups NOTIFY blockedLookupsChanged )
public:

	//! Constructor
	explicit ValueListItemListModel();

// Interface for the model.
public:

	//! Returns the value list.
	ValueListFront* valueList() { return m_valueList; }

	//! The current filter.
	const TypedValueFilter* filter() const { return m_filter.data(); }

	//! The currently selected lookup.
	QJsonValue selectedLookup() const { return m_selectedLookup; }

	//! Blocked lookups
	QJsonArray blockedLookups() const;

	//! Returns the number of rows under the given parent.
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

	//! Returns the data stored under the given role for the item referred to by the index.
	virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

	//! Role names. Note: The documentation claims that we should call setRoleNames to specify the roles. However, this function no longer exists and roleNAmes has been made virtula.
	virtual QHash< int,QByteArray > roleNames() const;

signals:

	//! Signaled when the value list has changed.
	void valueListChanged();

	//! Signaled when the filter changes.
	void filterChanged();

	//! Signaled when the selected lookup changes.
	void selectedLookupChanged();

	//! Signaled when the blocked lookups change.
	void blockedLookupsChanged();

public slots:

	//! Called when the reset of the model data is required.
	void resetFromList();

	//! Sets the value list.
	void setValueList( ValueListFront* valueList );

	//! Sets the filter.
	void setFilter( TypedValueFilter* filter );

	//! Sets the currently selected lookup.
	void setSelectedLookup( const QJsonValue& lookup );

	//! Sets the blocked lookups.
	void setBlockedLookups( const QJsonArray& blocked );

// Private interface.
private:

	//! Returns an array of value list items without the blocked lookups.
	AsyncFetch* filterBlocked( AsyncFetch* items ) const;

	//! Includes the selected lookup in the data if it is missing.
	void includeSelectedLookupIfMissing( bool notify );

	//! Gets the index of the item in the stored value list item or -1 if the item does not exist.
	int indexOf( int id ) const;

	//! Inserts lookup to the value list.
	void insertLookup( const QJsonValue& lookup, bool notify );

	//! Inserts lookup to the value list to the specified position.
	void insertLookup( int row, const QJsonValue& lookup, bool notify );

	/**
	 * @brief setData
	 * @param data The content for the value list.
	 */
	void setData( const QJsonArray& data );

	//! Returns data for display.
	QVariant forDisplay( const QModelIndex & index ) const;

	//! Returns data for decoration.
	QVariant forDecoration( const QModelIndex & index ) const;

	//! Returns data for lookup.
	QVariant forLookup( const QModelIndex & index ) const;

	//! Returns data for lookup.
	QVariant forId( const QModelIndex & index ) const;

// Private data.
private:

	ValueListFront* m_valueList;  //!< The value list used to populate the model.
	QPointer< TypedValueFilter > m_filter;  //!< Filter for fetching the lookups.

	QJsonArray m_data;  //!< A cached copy of the value list item data.
	QJsonValue m_selectedLookup;  //!< The lookup value that is currently selected.
	QHash< int, QJsonValue > m_blockedLookups;  //!< A collection loops that should not be shown in the listing.

};

#endif // VALUELISTItemListMODEL_H
