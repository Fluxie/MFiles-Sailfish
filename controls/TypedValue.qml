import QtQuick 2.0
import Sailfish.Silica 1.0

import "Components.js" as Components
import "TypedValue.js" as Logic

// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Loader {

	id: typedValue		

	// Properties
	property string propertyDefinitionName
	property variant propertyValue
	property real minimumHeight: Theme.itemSizeMedium
	property bool dynamicHeight: false

	// Updates the control when the property valeu changes.
	onPropertyValueChanged: {
		if( propertyValue ) {
			source = Logic.selectControl( propertyValue.TypedValue );
			minimumHeight = Logic.selectImplicitHeight( propertyValue.TypedValue );
		}
	}

	// Set the value after loading has been completed.
	onLoaded: {
		item.value = propertyValue.TypedValue;
		if( item.dynamicHeight )
			dynamicHeight = item.dynamicHeight;
	}

	// Bind
	Connections {
		target: typedValue.item
		ignoreUnknownSignals: true
		onHeightChanged: {
			if( typedValue.dynamicHeight )
				typedValue.height = item.height;
		}
	}
}
