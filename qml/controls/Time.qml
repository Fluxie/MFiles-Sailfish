import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

Button {

	id: time

	// Properties
    property variant propertyValue
    property VaultFront vault

    text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""

	// Action
	onClicked: {
		var dialog = pageStack.push("../dialogs/AccurateTimePickerDialog.qml" );
		dialog.accepted.connect(function() {
			time.text = "You chose: " + dialog.timeText
		})
	}
}
