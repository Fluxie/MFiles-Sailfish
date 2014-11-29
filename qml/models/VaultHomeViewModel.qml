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

import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0
import "../controls"
import "../common/listings.js" as ListingLogic

/*ListModel {

    id: listModel

    property VaultFront vault

    // Main listing.
    ListElement {

        property var listing: listModel.vault.rootListing
        property VaultFront vault: listModel.vault

        anchors.fill: parent
        clip: true
    }

    // Favorites
    ListElement {

        property var listing: listModel.vault.listing( '/views/v15/items' )
        property VaultFront vault: listModel.vault

        anchors.fill: parent
        clip: true
    }

}*/

VisualItemModel {

    id: itemModel

    property VaultFront vault
    property real itemWidth


    // Root.
    VaultHomeViewItem {

        id: rootListing

        title: vault.name
        listing: itemModel.vault.rootListing
        vault: itemModel.vault
    }

    // Assignments
    VaultHomeViewItem {

        id: assignedToListing

        // Refresh the listing if the authentication changes.
        Connections {

            target: itemModel.vault
            onAuthenticatedChanged: {
                var listing = itemModel.vault.listing( '/views/v9/items' );
                if( listing )
                    assignedToListing.listing = listing;
            }
        }

        title: 'My Assignments'
        vault: itemModel.vault
    }

    // Favorites
    VaultHomeViewItem {

        id: favoritesListing

        // Refresh the listing if the authentication changes.
        Connections {

            target: itemModel.vault
            onAuthenticatedChanged: {
                var listing = itemModel.vault.listing( '/views/v15/items' );
                if( listing )
                    favoritesListing.listing = listing;
            }
        }

        title: 'My Favorites'
        vault: itemModel.vault
    }
}

