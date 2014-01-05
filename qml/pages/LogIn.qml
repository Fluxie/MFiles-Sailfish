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
import "LogIn.js" as Logic
import "../common/structs.js" as Structs

Page {

	id: page
	property int textAlignment: Text.AlignLeft
	property Column newVaultInfo: null

	Component.onCompleted: {
		Logic.initialize( listView.listModel, newVaultInfo, addNewVaultButton );
	}

	// To enable PullDownMenu, place our content in a SilicaFlickable
	SilicaListView {
		id: listView
		property ListModel listModel: ListModel {}

		model: listModel
		anchors.fill: parent

		PullDownMenu {
			id: pullDownMenu

			MenuItem {
				id: addNewVaultButton
				text: "Add new Vault"
				onClicked: {
					page.newVaultInfo.visible = true;
				}
			}

			MenuItem {
				text: "Reset database"
				onClicked: {
					Logic.resetDatabase();
					Logic.initialize( listView.listModel, newVaultInfo, addNewVaultButton );
				}
			}
		}

		// Tell SilicaFlickable the height of its content.
		// TODO: Figure out the reason for this
		// contentHeight: childrenRect.height

		// Layout in a column
		header: Column {
			width: page.width
			spacing: Theme.paddingSmall
			PageHeader {
				title: "Log in"
			}

			// A second column to hold the login inputs so we can hide it
			// in the case we have previous vaults to show by default
			// The "new vault" pull down menu action should show this in that case
			LogInNewVaultDetails {
				id: newVaultInfo

				Component.onCompleted: {
					page.newVaultInfo = newVaultInfo;
				}

				onLogInRequested: {
					Logic.logInToNewVault(
						url, username, password, rememberPassword,
						function( err, vault ) {
							if( err ) {
								return newVaultInfo.showError( err );
							}

							// Append the vault to the vault list.
							// TODO: Keep list sorted by inserting vault to
							//       the correct spot.
							listView.listModel.append( vault );
							page.newVaultInfo.visible = false;
							page.newVaultInfo.clear();
						});
				}
			}
		}

		property LogInVaultDetails expandedItem: null
		delegate: LogInVaultDetails {
			id: listItem

			onExpanded: {
				if( listView.expandedItem !== null ) {
					listView.expandedItem.collapse();
				}
				listView.expandedItem = listItem;
			}

			onCollapsed: {
				listView.expandedItem = null;
			}

			onLogInRequested: {
				// Turn the model into a JS object and replace
				// the username with the one from the textbox
				var vault = new Structs.Vault( model );
				vault.username = username;

				Logic.logInToVault( vault, password, rememberPassword, function( err, vault ) {
					if( err ) {
						var msg = err.Message || err.message || 'Unknown error during log in';
						return listItem.showError( msg );
					} else {
						listView.listModel.set( index, vault );
					}
				});
			}
		}
	}
}


