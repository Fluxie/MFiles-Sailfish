import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

TextField {

	id: field

	// Value
	color: Theme.primaryColor
	text: propertyValue ? propertyValue.TypedValue.DisplayValue : ''
	validator: RealValidator {
		decimals: 2
		notation: "StandardNotation"
	}

	// Submit the changes when the active focus is lost.
	onActiveFocusChanged: {
		if( ! activeFocus )
		{
			var asNumber = Number.fromLocaleString( Qt.locale(), field.text );
			typedValue.submit( asNumber );
		}
	}
}
