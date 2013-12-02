import QtQuick 2.0
import Sailfish.Silica 1.0

import "../common/utils.js" as Utils

// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Column{

	id: lookups

	property variant value
	property variant lookupValues: {}

	onValueChanged: {
		if( value )
		{
			// Set the model for the repeater to display the property values.
			// The number of values displayed is limited.
			console.assert( value.DataType === 10, "Excepted multi-select lookup, got " + value.DataType );
			lookupValues = value.Lookups;
			if( lookupValues.length > 2 )
				content.model = 2;
			else
				content.model = lookupValues.length;
		}
	}

	// Position
	anchors.fill: parent

	// Lookup values
	Repeater {
		id: content

		Label {

			// Position
			anchors.left: parent.left
			anchors.right: parent.right
			height: Theme.itemSizeExtraSmall

			// Ccntent
			text: lookupValues[ index ].DisplayValue
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
			var listing = pageStack.push(
					'../pages/LookupListing.qml', { title: lookups.parent.propertyDefinitionName, typedValue: value } );
		}
	}

}



