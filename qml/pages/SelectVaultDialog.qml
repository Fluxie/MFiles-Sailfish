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

Page {
	id: page

	property ListModel listModel: ListModel {}

	property string vaultName
	property string vaultGuid
	property string vaultAuthentication

	signal accepted
	signal rejected

	allowedOrientations: Orientation.All

	SilicaListView {
		id: listView
		anchors.fill: parent
		model: parent.listModel

		header: PageHeader { title: 'Select Vault' }

		ViewPlaceholder {
			enabled: listView.count === 0
			text: 'You do not have access to any vaults'
		}

		VerticalScrollDecorator {}

		delegate: ListItem {
			id: listItem

			onClicked: {
				page.vaultName = model.name;
				page.vaultGuid = model.guid;
				page.vaultAuthentication = model.authentication
				page.accepted();
			}

			Label {
				text: model.name
				x: Theme.paddingLarge
			}
		}
	}
}
