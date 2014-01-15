import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils
import "TypedValue.js" as Logic


// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Loader {

	id: typedValue

	// Properties
	property string propertyDefinitionName
	property variant propertyValue
	property VaultFront vault
	property real minimumHeight: Theme.itemSizeMedium

	// Signals
	// Signaled when the property value selection as been submitted/accepted.
	signal accepted	

	// Updates the control when the property value changes.
	onPropertyValueChanged: {
		if( propertyValue ) {
			source = Logic.selectControl( propertyValue.TypedValue )
			minimumHeight = Logic.selectImplicitHeight( propertyValue.TypedValue )
		}
		else
		{
			// Clear the source.
			source = ""
		}
	}

	// Submits a new value to the typed value.
	// The actual controls should call this function to update the property value.
	function submit( newValue ) {

		// Update the property value.
		var newPropertyValue = Utils.deepCopy( typedValue.propertyValue );
		Logic.setTypedValue( newPropertyValue.TypedValue, newValue );
		typedValue.propertyValue = newPropertyValue
		accepted()  // Signal.
	}
}
