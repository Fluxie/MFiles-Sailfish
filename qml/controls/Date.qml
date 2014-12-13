import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Date.js" as Logic

Column {

	property alias label: propertyLabel.label

	// Position
	anchors {
		fill: parent
		leftMargin: Theme.paddingLarge;
	}

	BackgroundItem {

		id: date

		Label {

			id: dateLabel

			// Position.
			anchors.fill: parent

			// Text.
			verticalAlignment: Text.AlignVCenter
			text: Logic.getLabelText( propertyValue )
		}

		// Action
		onClicked: {
			var dt = new Date( propertyValue.TypedValue.Value );
			var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", {
											date: dt
										})
			dialog.accepted.connect(function () {

				// Submit.
				typedValue.submit( dialog.date );
			} )
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
