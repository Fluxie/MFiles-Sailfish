import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils


// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Column {

	id: lookups

	// Properties
	property variant propertyValue
	property int propertyDefinitionId: propertyValue ? propertyValue.PropertyDef : -1
	property variant typedValue: propertyValue ? propertyValue.TypedValue : undefined
	property VaultFront vault
	property bool propertyDefinitionsReady: vault ? vault.propertyDefinitionsReady : false
	property string propertyDefinitionName: ( propertyDefinitionsReady && propertyDefinitionId !== -1 ) ? vault.get( VaultFront.PropertyDefinition, propertyDefinitionId ).Name : ""

	property variant lookupValues: {

	}

	onTypedValueChanged: {
		if (typedValue) {
			// Set the model for the repeater to display the property values.
			// The number of values displayed is limited.
			console.assert(
						typedValue.DataType === 10,
						"Excepted multi-select lookup, got " + typedValue.DataType)
			lookupValues = typedValue.Lookups
			if (lookupValues.length > 2)
				content.model = 2
			else
				content.model = lookupValues.length
		}
	}

	// Position
	anchors.fill: parent

	// Lookup values
	Repeater {
		id: content

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
				text: lookupValues[index].DisplayValue
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
		visible: lookupValues.length > 2
		enabled: lookupValues.length > 2

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
