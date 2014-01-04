import QtQuick 2.0
import Sailfish.Silica 1.0

Button {

	id: time

	// Properties
    property Loader container
	property variant value
	onValueChanged: {
		text: value.DisplayValue
	}

	// Action
	onClicked: {
		var dialog = pageStack.push("../dialogs/AccurateTimePickerDialog.qml" );
		dialog.accepted.connect(function() {
			time.text = "You chose: " + dialog.timeText
		})
	}
}
