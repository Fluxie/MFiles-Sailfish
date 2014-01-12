import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "TypedValue.js" as Logic


// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Loader {

	id: typedValue

	// Properties
	property string propertyDefinitionName
	property variant propertyValue
	property VaultFront vault
	property real minimumHeight: Theme.itemSizeMedium

	// Updates the control when the property valeu changes.
	onPropertyValueChanged: {
		if (propertyValue) {
			source = Logic.selectControl(propertyValue.TypedValue)
			minimumHeight = Logic.selectImplicitHeight(propertyValue.TypedValue)
		}
	}

	// Set the value after loading has been completed.
	onLoaded: {
		propertyValueBinder.target = typedValue.item
		vaultBinder.target = typedValue.item
	}

	// Bind container to the selected control.
	Binding {

		id: propertyValueBinder

		property: "propertyValue"
		value: propertyValue
	}

	Binding {

		id: vaultBinder

		property: "vault"
		value: vault
	}
}
