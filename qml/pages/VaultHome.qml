/*
 *  Copyright 2013, 2014 Mikko Rantanen, Juha Lepola
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
import "VaultHome.js" as Logic
import "../common/listings.js" as ListingLogic


Page {

    id: homePage

	property VaultFront vault : VaultFront {}
	property string mfwaUrl: ''
	property string authentication: ''
    property string vaultName: ''

	function initialize() {
        Logic.initialize( homePage, listView );
	}	

	// To enable PullDownMenu, place our content in a SilicaFlickable
	SilicaListView {

		id: listView

        anchors.fill: parent
		clip: true

        header: PageHeader {

            id: header
            title: homePage.vaultName
        }

        PullDownMenu {
            id: pullDownMenu

            MenuItem {
                text: "Refresh"
                enabled: page.listing.status === ListingStatus.Ready
                onClicked: {
                    homePage.vault.rootListing.requestRefresh()
                }
            }
        }

		property ListModel favoritesList: ListModel {}
        model: QmlSortFilterProxyModel {

            id: sorter

            dynamicSortFilter: true

            sourceModel : ViewListModel {

                dataFilter: ViewListModel.AllItems
                listing: homePage.vault.rootListing
                vault: homePage.vault
            }

            // Set function for sorting the listing.
            lessThanJS: ListingLogic.lessThanFolderItem;
        }

		ViewPlaceholder {
			enabled: listView.count === 0
            text: page.listing.status === ListingStatus.Refreshing || !page.listing.empty ? 'Refreshing...' : 'No results'
		}

		delegate: ListItem {
			id: listItem

			onClicked: {
				var properties = pageStack.push(
                        'ViewListing.qml', { view: model.data, path: model.resource, vault: homePage.vault } );
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
