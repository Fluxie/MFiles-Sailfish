import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils


// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Column {

	id: lookups

	// Position
	anchors.fill: parent

	// Lookup values
	Repeater {
		id: content

		model: LookupModel {

			id: lookupModel

			rowLimit: 2
			propertyValue: typedValue.propertyValue
		}

		BackgroundItem {

			// Position
			anchors.left: parent.left
			anchors.right: parent.right
			height: Theme.itemSizeExtraSmall

			Label {

				// Position.
				anchors.fill: parent
				anchors.leftMargin: Theme.paddingLarge

				// Content
				verticalAlignment: Text.AlignVCenter
				text: model.display
			}
		}
	}

	// Button for opening lookup listing.
	Button {
		id: more

		// Position
		height: Theme.itemSizeExtraSmall
		anchors.horizontalCenter: parent.horizontalCenter

		// Visibility
		visible: lookupModel.lookupCount > lookupModel.rowLimit
		enabled: lookupModel.lookupCount > lookupModel.rowLimit

		// Content
		text: "More"

		// Action
		onClicked: {
			var listing = pageStack.push('../pages/LookupListing.qml', {
											 title: propertyDefinitionName,
											 typedValue: lookups.typedValue
										 })
		}
	}
}
