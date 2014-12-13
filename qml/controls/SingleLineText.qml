import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/structs.js" as Structs

TextField {

	id: field

	// Text
	color: Theme.primaryColor
	text: propertyValue ? propertyValue.TypedValue.DisplayValue : ''	
	placeholderText: field.label

	// Submit the changes when the active focus is lost.
	onActiveFocusChanged: {
		if( ! activeFocus )
			typedValue.submit( field.text );
	}
}
