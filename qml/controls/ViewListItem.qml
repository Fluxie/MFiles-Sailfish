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
import "../common/listings.js" as ListingLogic

ListItem {
    id: listItem

    property VaultFront vault
    property var view

    onClicked: {

        // Select action based on the type of the item.
        switch( model.type )
        {
        case ViewListModel.ViewFolder :
            pageStack.push( '../pages/ViewListing.qml', { view: model.data, path: model.resource, vault: listItem.vault } );
            break;

        case ViewListModel.ObjectVersion :
            pageStack.push( '../pages/PropertyValues.qml', { objectVersion: model.data, vault: listItem.vault } );
            break;

        case ViewListModel.PropertyFolder :
            pageStack.push( '../pages/ViewListing.qml', { view: listItem.view, propertyFolder: model.data, path: model.resource, vault: listItem.vault } );
            break;

        default:
            break;
        }
    }

    Column {
        Label {
            text: model.display
            x: Theme.paddingLarge
        }

        Label {
            text: model.resource
            x: Theme.paddingLarge * 2
        }
    }
}
