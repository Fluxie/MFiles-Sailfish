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
#include "../mfiles/view.h"
#include "../frontend/listingfront.h"

#include "viewlistmodeldataaccessor.h"

const int ViewListModel::ResourceRole = Qt::UserRole;
const int ViewListModel::DataRole = Qt::UserRole + 1;
const int ViewListModel::TypeRole = Qt::UserRole + 2;

ViewListModel::ViewListModel( QObject *parent ) :
	ListModelBase( parent ),
	m_filter( Undefined ),
	m_listing( nullptr ),
	m_listingDataSet( false ),
	m_listingDataUpdateCookie( 0 )
{
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
	// roles.insert( Qt::DecorationRole, "decoration" );
	roles.insert( ViewListModel::ResourceRole, "resource" );
	roles.insert( ViewListModel::DataRole, "data" );
	roles.insert( ViewListModel::TypeRole, "type" );
	return roles;
}

ListingStatus::Status ViewListModel::status() const
{
	if( m_listing == nullptr || m_listingDataSet == false )
		return ListingStatus::Refreshing;

	qDebug( "ViewListModel::status" );
	return m_listing->status();
}

LazyModelDataAccessor* ViewListModel::createAccessor( const QModelIndex& index )
{
	return new ViewListModelDataAccessor( this, index );
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

	// Verify that the listing and the vault are compatible.
	if( vault != nullptr && m_listing != nullptr )
	{
		Q_ASSERT( m_listing->isPartOf( vault ) );
	}

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

	// Verify that the listing and the vault are compatible.
	if( m_vault != nullptr && listing != nullptr )
	{
		Q_ASSERT( listing->isPartOf( m_vault ) );
	}

	// Disconnect previous signals.
	if( m_listing != nullptr )
	{
		m_listing->disconnect( this, nullptr );
		this->disconnect( m_listing, nullptr );
	}

	// Store the listing.
	m_listing = listing;
	if( m_listing != nullptr )
	{
		QObject::connect( m_listing, &ListingFront::refreshed, this, &ViewListModel::setListingData );
		QObject::connect( this, &ViewListModel::dataFilterChanged, m_listing, &ListingFront::requestRefresh );
		m_listing->requestRefresh();
	}
	emit listingChanged();
}

QJsonArray ViewListModel::acceptBuiltInViews( const QJsonArray& input )
{
	// Search for built-in views among the input and select them for output.
	QJsonArray output;
	foreach( const MFiles::FolderContentItem& item, input )
	{
		if( item.type() == MFiles::Constants::FolderContentItemType::ViewFolder )
		{
			MFiles::View view( item.view() );
			if( view.builtIn() )
				output.append( item.toJsonValue() );
		}
	}
	return output;
}

QJsonArray ViewListModel::acceptCommonViews( const QJsonArray& input, bool excludeBuiltInViews )
{
	// Search for common views among the input and select them for output.
	QJsonArray output;
	foreach( const MFiles::FolderContentItem& item, input )
	{
		if( item.type() == MFiles::Constants::FolderContentItemType::ViewFolder )
		{
			// Accept all common views. But don't accept built-in views if they were excluded.
			MFiles::View view( item.view() );
			if( view.common() && ! excludeBuiltInViews )
				output.append( item.toJsonValue() );
			else if( view.common() && excludeBuiltInViews && ! view.builtIn() )
				output.append( item.toJsonValue() );
		}
	}
	return output;
}

QJsonArray ViewListModel::acceptPrivateViews( const QJsonArray& input )
{
	// Search for private views among the input and select them for output.
	QJsonArray output;
	foreach( const MFiles::FolderContentItem& item, input )
	{
		if( item.type() == MFiles::Constants::FolderContentItemType::ViewFolder )
		{
			// Accept all private views. But don't accept built-in views if they were excluded.
			MFiles::View view( item.view() );
			if( ! view.common() )
				output.append( item.toJsonValue() );
		}
	}
	return output;
}

void ViewListModel::forDisplay( const QModelIndex & index, QVariant& variant ) const
{	
	// Return the display name of the item.
	MFiles::FolderContentItem item( m_listingData[ index.row() ] );
	variant = item.displayName();
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

void ViewListModel::setListingData( const QJsonArray& data )
{
	// Apply filter.
	QJsonArray filteredData;
	switch( m_filter )
	{
	case DataFilter::AllItems :
		filteredData = data;
		break;

	case DataFilter::BuiltInViews :
		filteredData = acceptBuiltInViews( data );
		break;

	case DataFilter::CustomCommonViews :
		filteredData = acceptCommonViews( data, true );
		break;

	default:
		qCritical( "TODO: Report error." );
		break;
	};

	// Save the data.
	{
		ResetModel reset( this );
		Q_UNUSED( reset );

		// Set the data.
		qDebug( "ViewListModel::setListingData" );
		m_listingData = filteredData;
		m_listingDataSet = true;
	}
	emit statusChanged();
}


