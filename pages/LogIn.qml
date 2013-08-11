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

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: "Add new Vault"
                onClicked: pageStack.push(Qt.resolvedUrl("VaultConfig.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
		// TODO: Figure out the reason for this
        contentHeight: childrenRect.height

		// Layout in a column
        Column {
            width: page.width
            spacing: Theme.paddingSmall
            PageHeader {
                title: "M-Files"
            }

			// A second column to hold the login inputs so we can hide it
			// in the case we have previous vaults to show by default
			// The "new vault" pull down menu action should show this in that case
            Column {
                id: newVaultInfo
                width: page.width
                spacing: Theme.paddingSmall

				function doLogIn() {
					Logic.selectVault( url.text, username.text, password.text, function( err, vault ) {
						if( err ) {
							error.visible = true;
							error.text = err.message;
							return;
						}

						if( vault ) {
							Logic.saveVault( vault );
							Logic.useVault( vault );
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
						newVaultInfo.doLogIn();
                    }
                }
                Button {
                    text: "Log in"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
						newVaultInfo.doLogIn();
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
    }
}


