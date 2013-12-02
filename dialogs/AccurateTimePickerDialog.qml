import QtQuick 2.0
import Sailfish.Silica 1.0

import "AccurateTimePicker.js" as Logic

Dialog {

	id: accurateTimePicker

	property alias hour: clock.hour
	property alias hourMode: clock.hourMode
	property alias minute: clock.minute
	property int second
	property var time: new Date()
	property string timeText: ''

	onHourChanged: Logic.synchronizeDialogToSatelliteData( accurateTimePicker )
	onMinuteChanged: Logic.synchronizeDialogToSatelliteData( accurateTimePicker )
	onSecondChanged: Logic.synchronizeDialogToSatelliteData( accurateTimePicker );
	onTimeTextChanged: header.acceptText = timeText;


	DialogHeader {

		id: header

		anchors.top: parent.top
		anchors.right: parent.right

		acceptText: ''
	}

	// Clock
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

		onValueChanged: accurateTimePicker.second = value;
	}
}
