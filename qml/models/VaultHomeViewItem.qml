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

Item {

    id: itemHost

    // Properties
    property alias title: listingHeader.title
    property alias listing: viewListing.listing
    property alias vault: viewListing.vault

    //Positioning.
    anchors {

        top: parent.top
        bottom: parent.bottom
    }
    width: itemModel.itemWidth


    // Header for this view..
    PageHeader {

        id: listingHeader
    }

    View {

        id: viewListing


        anchors {

            top: listingHeader.bottom
            left: itemHost.left
            right: itemHost.right
            bottom: itemHost.bottom
        }
    }
}
