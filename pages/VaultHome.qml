/*
 *  Copyright 2013 Mikko Rantanen
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
import Sailfish.Silica.theme 1.0
import mohari.sailfish 1.0
import "VaultHome.js" as Logic


Page {

	id: page

	property VaultFront vault : VaultFront {}
	property string mfwaUrl: ''
	property string authentication: ''
	property string vaultName: ''

	function initialize() {
		Logic.initialize( page, listView );
	}

	Connections {
		target : vault
		onAllCachesPopulated : {

			// Update the names
			var lvm = listView.model;
			for(var i=0; i < lvm.count; i++) {
				var item = lvm.get(i);
				var mfclass = vault.get( VaultFront.Class, item.data.Class );
				var mfot = vault.get( VaultFront.ObjectType, item.data.ObjVer.Type );
				item.className = mfclass.Name;
				item.objectTypeName = mfot.Name;
			}
		}
	}


	PageHeader {
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right

		id: header
		title: page.vaultName
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
		model: favoritesList

		ViewPlaceholder {
			enabled: listView.count === 0
			text: 'No favorites'
		}

		delegate: ListItem {
			id: listItem

			onClicked: {
				var properties = pageStack.push(
							'PropertyValues.qml', { objectVersion: model.data, vault: page.vault } );
			}

			Column {
				Label {
					text: model.title
					x: Theme.paddingLarge
				}

				Label {
					text: model.objectTypeName + ':' + model.className
					x: Theme.paddingLarge * 2
				}
			}
		}
	}
}
