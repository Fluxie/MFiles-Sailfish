import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.theme 1.0

Column {
	id: newVaultInfo
	width: parent.width
	visible: false
	spacing: Theme.paddingSmall

	signal logInRequested( string url, string username, string password, bool rememberPassword )

	function _triggerLogIn() {
		logInRequested( url.text, username.text, password.text, rememberPasswordChoice.checked );
	}

	function showError( err ) {
		errorMsg.visible = true;
		errorMsg.text = err.message || err.Message || err;
	}

	function clear() {
		url.text = '';
		username.text = '';
		password.text = '';

		errorMsg.visible = false;
		errorMsg.text = '';
	}


	TextField {
		id: url
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
			newVaultInfo._triggerLogIn();
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
			newVaultInfo._triggerLogIn();
		}
	}
	TextField {
		id: errorMsg
		width: parent.width
		horizontalAlignment: Text.AlignCenter
		visible: false
	}
}
