import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Date.js" as Logic

BackgroundItem {

	id: date

	Label {

		id: dateLabel

		// Position.
		anchors.fill: parent
		anchors.leftMargin: Theme.paddingLarge

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
			dateLabel.text = Logic.getLabelText( dialog.date );
		})
	}
}
