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

#include "viewlistmodel.h"

#include "resetmodel.h"
#include "../backend/asyncfetch.h"
#include "../mfiles/foldercontentitem.h"
#include "../mfiles/objectversion.h"
#include "../mfiles/objver.h"
#include "../mfiles/typedvalue.h"
#include "../frontend/listingfront.h"

const int ViewListModel::ResourceRole = Qt::UserRole;
const int ViewListModel::DataRole = Qt::UserRole + 1;
const int ViewListModel::TypeRole = Qt::UserRole + 2;

ViewListModel::ViewListModel( QObject *parent ) :
	ListModelBase( parent ),
	m_filter( Undefined ),
	m_listing( nullptr ),
	m_listingDataUpdateCookie( 0 )
{
	// Make internal connections.
	QObject::connect( this, &ViewListModel::dataFilterChanged, this, &ViewListModel::refreshListingData );
	QObject::connect( this, &ViewListModel::listingChanged, this, &ViewListModel::refreshListingData );
}

//! Returns the number of rows under the given parent.
int ViewListModel::rowCount( const QModelIndex& ) const
{
	// Report the row count.
	return m_listingData.count();
}

//! Returns the data stored under the given role for the item referred to by the index.
QVariant ViewListModel::data( const QModelIndex& index, int role ) const
{
	// Return the role.
	QVariant data;
	switch( role )
	{
	case Qt::DisplayRole :
		this->forDisplay( index, data );
		break;

	case ViewListModel::ResourceRole :
		this->forResource( index, data );
		break;

	case ViewListModel::DataRole :
		this->forData( index, data );
		break;

	case ViewListModel::TypeRole :
		this->forType( index, data );
		break;

	default:
		qDebug( QString( "Unknown role %1").arg( role ).toStdString().c_str() );
	}
	return data;
}

//! Role names.
//! Note: The documentation claims that we should call setRoleNames to specify the roles.
//! However, this function no longer exists and roleNAmes has been made virtula.
QHash< int, QByteArray > ViewListModel::roleNames() const
{
	// Construct QHash to describe the roles and return it.
	// TODO: Should we reset the original roles too here?
	QHash< int, QByteArray > roles;
	roles.insert( Qt::DisplayRole, "display" );
	roles.insert( Qt::DecorationRole, "decoration" );
	roles.insert( ViewListModel::ResourceRole, "resource" );
	roles.insert( ViewListModel::DataRole, "data" );
	roles.insert( ViewListModel::TypeRole, "type" );
	return roles;
}


void ViewListModel::setDataFilter( DataFilter filter )
{
	// Something changed?
	if( m_filter == filter )
		return;

	m_filter = filter;
	emit dataFilterChanged();
}


void ViewListModel::setVault( VaultFront* vault )
{
	// Something changed?
	if( m_vault == vault )
		return;

	m_vault = vault;	
	emit vaultChanged();
}

/**
 * @brief Sets the listing the model represents.
 * @param listing New listing.
 */
void ViewListModel::setListing( ListingFront* listing )
{
	// Something changed?
	if( m_listing == listing )
		return;

	// Disconnect previous signals.
	if( m_listing != nullptr )
		m_listing->disconnect( this, nullptr );

	m_listing = listing;
	if( m_listing != nullptr )
		QObject::connect( m_listing, &ListingFront::refreshed, this, &ViewListModel::refreshListingData );
	emit listingChanged();
}

void ViewListModel::forDisplay( const QModelIndex & index, QVariant& variant ) const
{	
	// Return the display name of the item.
	MFiles::FolderContentItem item( m_listingData[ index.row() ] );
	variant = item.dipslayName();
}

void ViewListModel::forResource( const QModelIndex & index, QVariant& variant ) const
{
	MFiles::FolderContentItem item( m_listingData[ index.row() ] );
	switch( item.type() )
	{
	case MFiles::Constants::ViewFolder :
		{
			QJsonValue view = item.view();
			QString resource( "%1v%2/items" );
			QString args = resource.arg( this->m_listing->resource().replace( "items", "" ) ).arg( ( int ) view.toObject()[ "ID" ].toDouble() );
			variant = args;
		}
		break;

	case MFiles::Constants::ObjectVersion :
		{
			MFiles::ObjectVersion oversion( item.objectVersion() );
			MFiles::ObjVer objver( oversion.objVer() );
			QString resource( "/objects/%1/%2/%3" );
			QString withArgs = resource.arg( objver.type() ).arg( objver.id() ).arg( objver.version() );
			variant = withArgs;
		}
		break;

	case MFiles::Constants::PropertyFolder :
		{
			MFiles::TypedValue pv( item.propertyFolder() );
			QString resource( "%1%2/items" );
			QString args = resource.arg( this->m_listing->resource().replace( "items", "" ) ).arg( pv.getUriEncodedValue() );
			variant = args;
		}
		break;

	default :
		qCritical( "ViewListModel::forResource" );
		break;
	}
}

void ViewListModel::forData( const QModelIndex & index, QVariant& variant ) const
{
	MFiles::FolderContentItem item( m_listingData[ index.row() ] );
	switch( item.type() )
	{
	case MFiles::Constants::ViewFolder :
		variant = item.view();
		break;

	case MFiles::Constants::ObjectVersion :
		variant = item.objectVersion();
		break;

	case MFiles::Constants::PropertyFolder :
		variant = item.propertyFolder();
		break;

	default :
		qCritical( "ViewListModel::forResource" );
		break;
	}
}

void ViewListModel::forType( const QModelIndex & index, QVariant& variant ) const
{
	MFiles::FolderContentItem item( m_listingData[ index.row() ] );
	variant = item.type();
}

/**
 * @brief Refreshes the listing data.
 */
void ViewListModel::refreshListingData()
{
	// Can we fill the listing data?
	if( m_listing == nullptr || m_filter == DataFilter::Undefined )
	{
		// Reset model.
		ResetModel reset( this );
		Q_UNUSED( reset );

		// Clear the previous listing.
		m_listingData = QJsonArray();

		return;
	}

	// Fetch the listing and apply appropriate filter.
	AsyncFetch* fetchListing = m_listing->list();
	switch( m_filter )
	{
	case DataFilter::AllItems :
		break;
	default:
		qCritical( "TODO: Report error." );
		break;
	};

	// Set the listing.
	int cookie = ++m_listingDataUpdateCookie;
	QObject::connect( fetchListing, &AsyncFetch::finished,  [=]() {

		// We are doing the update asynchronously.
		// Ensure that the latest listing request is the one that takes effect.
		if( this->m_listingDataUpdateCookie != cookie )
			return;

		// Reset model.
		ResetModel reset( this );
		Q_UNUSED( reset );

		// Set the data.
		m_listingData = fetchListing->values();

		// Mark the fetch for deletion.
		fetchListing->deleteLater();

	} );  // end connect
}
