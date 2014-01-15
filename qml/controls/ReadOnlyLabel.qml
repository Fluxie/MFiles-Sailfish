import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

SilicaFlickable {

	id: extendedText
	contentHeight: Theme.itemSizeExtraSmall
	contentWidth: parent.width
	flickableDirection: Flickable.HorizontalFlick

	Label {

		// Position.
		anchors.fill: parent
		anchors.leftMargin: Theme.paddingLarge

		// Text.
		verticalAlignment: Text.AlignVCenter
		text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
	}

	HorizontalScrollDecorator {
		flickable: extendedText
	}
}
