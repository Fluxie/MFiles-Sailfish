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
import "LogIn.js" as Logic


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
            Column {
                id: newVaultInfo
                width: page.width
                visible: false
                spacing: Theme.paddingSmall

                Component.onCompleted: {
                    page.newVaultInfo = newVaultInfo;
                }

				function logInNewVault() {
					Logic.selectVault( url.text, username.text, password.text, function( err, vault ) {
						if( err ) {
							error.visible = true;
							error.text = err.message;
							return;
						}

						if( vault ) {
							Logic.saveVault( vault );
							Logic.useVault( vault, true );
						}
					})
				}

                TextField {
                    id: url
                    text: "http://192.168.0.103/m-files"
                    width: parent.width
                    label: "Web Access URL"
                    placeholderText: "M-Files Web Access URL"
                    focus: true
                    horizontalAlignment: textAlignment
                    EnterKey.onClicked: {
                        username.focus = true;
                    }
                }
                TextField {
                    id: username
                    text: "test"
                    width: parent.width
                    label: "Username"
                    placeholderText: "Username"
                    horizontalAlignment: textAlignment
                    EnterKey.onClicked: {
                        password.focus = true;
                    }
                }
                TextField {
                    id: password
                    echoMode: TextInput.Password
					text: "test"
                    width: parent.width
                    label: "Password"
                    placeholderText: "Password"
                    horizontalAlignment: textAlignment
                    EnterKey.onClicked: {
						newVaultInfo.logInNewVault();
                    }
                }
                TextSwitch {
                    id: rememberPasswordChoice
                    text: "Remember password"
                }

                Button {
                    text: "Log in"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
						newVaultInfo.logInNewVault();
                    }
                }
				TextField {
					id: error
					width: parent.width
					horizontalAlignment: Text.AlignCenter
					visible: false
				}
            }
        }

        delegate: ListItem {
            id: listItem

            onClicked: {
                Logic.useVault( model, false );
            }

            Column {
                Label {
                    text: model.name
                    x: Theme.paddingLarge
                }

                Label {
                    text: model.username
                    x: Theme.paddingLarge * 2
                }
            }
        }
    }
}


