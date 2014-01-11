import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

BackgroundItem {

	id: date

	// Properties
    property variant propertyValue
    property VaultFront vault

    Label{

        // Position.
        anchors.fill: parent
        anchors.leftMargin: Theme.paddingLarge

        // Text.
        verticalAlignment: Text.AlignVCenter
        text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
    }

	// Action
	onClicked: {
		var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", {
			date: new Date('2012/11/23')
		})
		dialog.accepted.connect(function() {
			date.text = "You chose: " + dialog.dateText
		})
	}
}
