import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../visuals"

Item {

	property alias label: propertyLabel.text
	property alias labelVisible: propertyLabel.visible
	property bool active: false

	height: Theme.itemSizeExtraSmall

	// This separator replaces the original background separator of the TextArea control.
	Separator {

		id: separator

		// Position.
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
			bottom: propertyLabel.top
		}

		// Color
		color: Theme.primaryColor
	}

	Label {

		id: propertyLabel

		// Position.
		anchors {
			top: separator.bottom
			left: parent.left
			right: parent.right
			bottom: parent.bottom
		}
		color: active ? Theme.primaryColor : Theme.secondaryColor
	}

}
