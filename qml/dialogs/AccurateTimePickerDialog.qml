import QtQuick 2.0
import Sailfish.Silica 1.0

import "AccurateTimePicker.js" as Logic
import "../common/utils.js" as Utils

Dialog {

	id: accurateTimePicker

	// Properties, alias the clock properties directly to controls.
	property alias hour: clock.hour
	property alias hourMode: clock.hourMode
	property alias minute: clock.minute
	property alias second: seconds.value
	property string timeText: ''
	property string timeFormat: Utils.getTimeFormat()

	onHourChanged: Logic.synchronizeDialogToSatelliteData( accurateTimePicker )
	onMinuteChanged: Logic.synchronizeDialogToSatelliteData( accurateTimePicker )
	onSecondChanged: Logic.synchronizeDialogToSatelliteData( accurateTimePicker )
	onTimeTextChanged: header.acceptText = timeText

	// Synchronize the data after the dialog has been loaded.
	Component.onCompleted: Logic.synchronizeDialogToSatelliteData( accurateTimePicker )

	DialogHeader {

		id: header

		anchors.top: parent.top
		anchors.right: parent.right

		acceptText: ''
	}

	// Clock, hours and minutes
	TimePicker {

		id: clock

		// Position
		anchors.top: header.bottom
		anchors.horizontalCenter: parent.horizontalCenter
	}

	// Seconds
	Slider {

		id: seconds

		// Position
		anchors.top: clock.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		// Range for seconds
		minimumValue: 0
		maximumValue: 59
		stepSize: 1
	}
}
