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
import "../controls"
import "../models"
import "../common/listings.js" as ListingLogic


Page {

	id: homePage

	property VaultFront vault : VaultFront {}
	property string mfwaUrl: ''
	property string authentication: ''
	property string vaultName: ''

	function initialize() {
		Logic.initialize( homePage );
		//favorites.vault = homePage.vault;
	}

	allowedOrientations: Orientation.All

	/*FavoritesListing {
		id: favorites

	}*/
	SilicaFlickable {

		id: host

		clip: true
		anchors.fill: parent

		PullDownMenu {
			id: pullDownMenu

			property var parentListing

			MenuItem {
				text: "Refresh"
				onClicked: {
					views.requestRefresh();
				}
			}

			MenuItem {
				text: "Home"
				onClicked: {

					var homePage = pageStack.find( function( searchFor ) {
						var found = searchFor.vaultName ? true : false;
						return found; } );
					pageStack.pop( homePage );
				}
			}

		}

		SlideshowView {

			id: slideShow

			clip: true
			/*anchors {

				top: host.top
				left: host.left
				right: host.right
				bottom: host.bottom
			}*/
			anchors.fill: parent

			itemWidth: width

			model: VaultHomeViewModel {

				id: views

				itemWidth: slideShow.itemWidth
				vault: homePage.vault

			}
		}
	}
}
