import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

BackgroundItem {

	id: time

	// Properties
    property variant propertyValue
    property VaultFront vault

    Label {

        id: timeLabel

        // Position.
        anchors.fill: parent
        anchors.leftMargin: Theme.paddingLarge

        // Text.
        verticalAlignment: Text.AlignVCenter
        text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
    }

	// Action
	onClicked: {

        // // This is available in all editors.
        var dt = new Date( propertyValue.TypedValue.Value );
        var dialog = pageStack.push("../dialogs/AccurateTimePickerDialog.qml", {
                                        hour: dt.getUTCHours(), minute: dt.getUTCMinutes(), second: dt.getUTCSeconds() } );
		dialog.accepted.connect(function() {
            timeLabel.text = dialog.timeText
            // TODO: Update the property value.
		})
	}
}
