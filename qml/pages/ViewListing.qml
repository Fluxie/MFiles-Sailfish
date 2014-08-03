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


Page {

    id: page

    property VaultFront vault : VaultFront {}
    property var view
    property var propertyFolder
    property string path


    PageHeader {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        id: header
        title:  page.propertyFolder ? page.propertyFolder.DisplayValue : page.view.Name
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {

        id: listView

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        property ListModel favoritesList: ListModel {}
        model: ViewListModel {


            dataFilter: ViewListModel.AllItems
            listing: page.vault.listing( page.path )
            vault: page.vault
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: 'No favorites'
        }

        delegate: ListItem {
            id: listItem

            onClicked: {

                // Select action based on the type of the item.
                switch( model.type )
                {
                case ViewListModel.ViewFolder :
                    pageStack.push( 'ViewListing.qml', { view: model.data, path: model.resource, vault: page.vault } );
                    break;

                case ViewListModel.ObjectVersion :
                    pageStack.push( 'PropertyValues.qml', { objectVersion: model.data, vault: page.vault } );
                    break;

                case ViewListModel.PropertyFolder :
                    pageStack.push( 'ViewListing.qml', { view: page.view, propertyFolder: model.data, path: model.resource, vault: page.vault } );
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
    }
}