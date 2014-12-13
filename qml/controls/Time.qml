import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

Column {

	property alias label: propertyLabel.label

	// Position
	anchors {
		fill: parent
		leftMargin: Theme.paddingLarge;
	}

	BackgroundItem {

		id: time

		Label {

			id: timeLabel

			// Position.
			anchors.fill: parent

			// Text.
			verticalAlignment: Text.AlignVCenter
			text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
		}

		// Action
		onClicked: {

			// // This is available in all editors.
			var dt = new Date(propertyValue.TypedValue.Value)
			var dialog = pageStack.push("../dialogs/AccurateTimePickerDialog.qml", {
											hour: dt.getUTCHours(),
											minute: dt.getUTCMinutes(),
											second: dt.getUTCSeconds()
										})
			dialog.accepted.connect(function () {

				// Construct the date object based on the previous value.
				var dt = new Date( propertyValue.TypedValue.Value );
				dt.setUTCHours( dialog.hour );
				dt.setUTCMinutes( dialog.minute );
				dt.setUTCSeconds( dialog.second );

				// Submit the value.
				typedValue.submit( dt );
			})
		}
	}

	PropertyLabel {

		id: propertyLabel

		// Position.
		anchors {
			left: parent.left
			right: parent.right
		}
	}
}
