import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
	id: listItem
	width: ListView.view.width
	clip: true

	// Define signals
	signal expanded()
	signal collapsed()
	signal logInRequested( string username, string password, bool rememberPassword )

	function _triggerLogIn() {
		logInRequested( username.text, password.text, rememberPasswordChoice.checked )
	}

	function expand() {
		listItem.state = 'expanded';
		listItem.expanded();
	}

	function collapse() {
		listItem.state = 'collapsed';
		listItem.collapsed();
	}

	function showError( msg ) {
		errorMsg.visible = true;
		errorMsg.text = msg;
	}

	state: 'collapsed'

	states: [
		State {
			name: 'expanded'
			PropertyChanges {
				target: listItem;
				height: vaultNameLabel.height + usernameLabel.height + details.height
			}
			PropertyChanges {
				target: expandButton;
				icon.source: 'image://theme/icon-l-up'
			}
		},
		State {
			name: 'collapsed'
			PropertyChanges {
				target: listItem;
				height: vaultNameLabel.height + usernameLabel.height
			}
			PropertyChanges {
				target: expandButton;
				icon.source: 'image://theme/icon-l-down'
			}
		}
	]

	transitions: Transition {
		NumberAnimation { properties: 'height'; easing.type: Easing.InOutQuad }
	}

	onClicked: {
		if( !model.authentication ) {
			expand();
		} else {
			listItem._triggerLogIn();
		}
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

	IconButton {
		id: expandButton
		anchors.right: parent.right
		onClicked: {
			if( listItem.state === 'expanded' ) {
				collapse();
			} else {
				expand();
			}
		}
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
			label: "Username"
			placeholderText: "Username"
			width: parent.width
			horizontalAlignment: textAlignment
			EnterKey.onClicked: {
				password.focus = true;
			}
		}
		TextField {
			id: password
			echoMode: TextInput.Password
			label: "Password"
			placeholderText: model.authentication ? '(Password remembered)' : 'Enter password'
			width: parent.width
			horizontalAlignment: textAlignment
			EnterKey.onClicked: {
				listItem._triggerLogIn();
			}
		}
		TextSwitch {
			id: rememberPasswordChoice
			checked: model.authentication
			text: "Remember password"
		}
		Button {
			id: logInButton
			text: "Log in"
			anchors.horizontalCenter: parent.horizontalCenter
			onClicked: {
				listItem._triggerLogIn();
			}
		}

		Label { id: errorMsg }
	}
}
