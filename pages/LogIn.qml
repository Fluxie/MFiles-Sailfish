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

				function logInToNewVault() {
                    Logic.logInToNewVault(
                        url.text,
                        username.text,
                        password.text,
                        rememberPasswordChoice.checked,
                        function( err ) {
                            if( err ) {
                                error.visible = true;
                                error.text = err.message;
                            }
                        });
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
						newVaultInfo.logInToNewVault();
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
						newVaultInfo.logInToNewVault();
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
            width: ListView.view.width
            clip: true

            function logInToVault() {

                Logic.logInToVault(
                    {
                        id: model.id,
                        name: model.name,
                        url: model.url,
                        guid: model.guid,
                        username: username.text,
                        authentication: model.authentication
                    },
                    password.text,
                    rememberPasswordChoice.checked,
                    function( err ) {
                        if( err ) {
                            details.visible = true;
                            errorMsg.visible = true;
                            errorMsg.text = err.Message;
                        }
                    });
            }

            onClicked: {
                if( !model.authentication ) {
                    console.log( "FFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUU" );
                    console.log( page.width );
                    console.log( listItem.width );
                    console.log( details.width );
                    expandItem.start();
                } else {
                    logInToVault();
                }
            }

            PropertyAnimation {
                id: expandItem
                target: listItem
                properties: "contentHeight"
                to: 450
                duration: 100
            }

            Label {
                id: vaultNameLabel
                text: model.name
                x: Theme.paddingLarge
            }

            Label {
                id: usernameLabel
                text: model.username
                x: Theme.paddingLarge * 2
                anchors.top: vaultNameLabel.bottom
            }

            Column {
                id: details
                anchors.top: usernameLabel.bottom
                x: Theme.paddingLarge * 3
                width: parent.width - x
                visible: true

                TextField {
                    id: username
                    text: model.username;
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
                    width: parent.width
                    label: "Password"
                    placeholderText: "Password"
                    horizontalAlignment: textAlignment
                    EnterKey.onClicked: {
                        listItem.logInToVault();
                    }
                }
                TextSwitch {
                    id: rememberPasswordChoice
                    checked: model.authentication
                    width: parent.width
                    text: "Remember password"
                }
                Button {
                    text: "Log in"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        console.log( "STAB" );
                        listItem.logInToVault();
                    }
                }

                Label { id: errorMsg }
            }
        }
    }
}


