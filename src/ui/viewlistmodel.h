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

#ifndef VIEWLISTMODEL_H
#define VIEWLISTMODEL_H

#include <QJsonArray>
#include <QObject>

#include "dataaccessorsource.h"
#include "listmodelbase.h"
#include "../frontend/listingstatus.h"

// Forward declarations.
class ListingFront;
class VaultFront;
class ViewListModelDataAccessor;

/**
 * @brief The ViewListModel class
 */
class ViewListModel : public ListModelBase, public DataAccessorSource
{
	/**
	 * @brief Resource role identifies the resource that can be used to fetch more information about the item.
	 */
	static const int ResourceRole;

	/**
	 * @brief Data role represents the actual item as a Json value.
	 */
	static const int DataRole;

	/**
	 * @brief Type role represents the type of the item.
	 */
	static const int TypeRole;

    Q_OBJECT
	Q_ENUMS( DataFilter )
	Q_ENUMS( ItemType )
	Q_PROPERTY( DataFilter dataFilter READ dataFilter WRITE setDataFilter NOTIFY dataFilterChanged )
	Q_PROPERTY( VaultFront* vault READ vault WRITE setVault NOTIFY vaultChanged )
	Q_PROPERTY( ListingFront* listing READ listing WRITE setListing NOTIFY listingChanged )
	Q_PROPERTY( ListingStatus::Status status READ status NOTIFY statusChanged )
public:

	enum DataFilter
	{
		Undefined,  //!< No data has been specified to be shown in the model.
		ViewsOnly,  //!< Only views are shon
		ObjectsOnly,  //!< Only objects are shown.
		AllItems  //!< All items are shown.
	};

	enum ItemType
	{
		Unknown	= 0,
		ViewFolder = 1,
		PropertyFolder = 2,
		TraditionalFolder = 3,
		ObjectVersion = 4,
	};

    /**
     * @brief ViewListModel
     * @param parent
     */
    explicit ViewListModel(QObject *parent = 0);

	/**
	 * @brief Gets the current data filter.
	 * @return The data filter of this model
	 */
	DataFilter dataFilter() const { return m_filter; }

	/**
	 * @brief vault
	 * @return Vault associated with this model.
	 */
	VaultFront* vault() const { return m_vault; }

	/**
	 * @brief listing that this model represents.
	 * @return Reference to the listing.
	 */
	ListingFront* listing() const { return m_listing; }

	/**
	 * @brief Gets the status of the model. Not necessarily the same as the status of the underlying listing.
	 * @return Status of the listing.
	 */
	ListingStatus::Status status() const;


// Interface implementing the model.
public:

    //! Returns the number of rows under the given parent.
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

    //! Returns the data stored under the given role for the item referred to by the index.
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    //! Role names. Note: The documentation claims that we should call setRoleNames to specify the roles. However, this function no longer exists and roleNAmes has been made virtula.
    virtual QHash< int,QByteArray > roleNames() const;

// DataAccessorSource.
public:

	/**
	 * @brief Creates new data accessor.
	 * @return New data accessor
	 */
	virtual LazyModelDataAccessor* createAccessor( const QModelIndex& index );

signals:

	/**
	 * @brief dataFilterChanged is signaled when the data filter changes.
	 */
	void dataFilterChanged();


	/**
	 * @brief vaultChanged is signaled when the vault associated with the property value model changes.
	 */
	void vaultChanged();

	/**
	 * @brief listingChanged is signaled when the listing associated with the model changes.
	 */
	void listingChanged();

	/**
	 * @brief statusChanged is signaled when the status of the listing changes.
	 */
	void statusChanged();

public slots:

	/**
	 * @brief Sets the data filter for the model.
	 * @param filter New data filter
	 */
	void setDataFilter( DataFilter filter );

	/**
	 * @brief setVault
	 * @param vault associated with the model.
	 */
	void setVault( VaultFront* vault );

	/**
	 * @brief Sets the listing the model represents.
	 * @param listing New listing.
	 */
	void setListing( ListingFront* listing );

// Private interface.
private:

	/**
	 * @brief forDisplay The display value.
	 * @param index The index of the value
	 * @param variant The value for display
	 */
	void forDisplay( const QModelIndex & index, QVariant& variant ) const;

	/**
	 * @brief forResource The resource path for fetching more information about the item.
	 * @param index The index of the resource
	 * @param variant The value for resource
	 */
	void forResource( const QModelIndex & index, QVariant& variant ) const;

	/**
	 * @brief forData The data as Json object.
	 * @param index The index of the data object
	 * @param variant The value for the data object.
	 */
	void forData( const QModelIndex & index, QVariant& variant ) const;

	/**
	 * @brief forData The type of the item.
	 * @param index The index of the type
	 * @param variant The type
	 */
	void forType( const QModelIndex & index, QVariant& variant ) const;

// Private slots.
private slots:	

	/**
	 * @brief setListingDate
	 * @param data New listing data.
	 */
	void setListingData( const QJsonArray& data );

// Private data:
private:

	friend class ViewListModelDataAccessor;

	DataFilter m_filter;  //!< The type of the data modelled in this model.
	QJsonArray m_listingData;  //!< The list contents.
	bool m_listingDataSet;
	ListingFront* m_listing;	
	int m_listingDataUpdateCookie;

	// Auxilary functions.
	VaultFront* m_vault;
};

#endif // VIEWLISTMODEL_H
