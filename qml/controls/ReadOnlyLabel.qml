import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

SilicaFlickable {

	id: extendedText
	contentHeight: Theme.itemSizeExtraSmall * 2
	contentWidth: parent.width
	flickableDirection: Flickable.HorizontalFlick
	property alias label: propertyLabel.label

	// Position
	anchors {
		fill: parent
		leftMargin: Theme.paddingLarge;
	}

	Label {

		id: labelText

		height: Theme.itemSizeExtraSmall

		// Position.
		// Position.
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
			bottom: propertyLabel.top
		}

		// Text.
		verticalAlignment: Text.AlignVCenter
		text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
	}

	PropertyLabel {

		id: propertyLabel

		// Position.
		anchors {
			top: labelText.bottom
			left: parent.left
			right: parent.right
			bottom: parent.bottom
		}
	}

	HorizontalScrollDecorator {
		flickable: extendedText
	}
}
