import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

TextField {

	// Properties
	property variant propertyValue
	property VaultFront vault

	// Value
	color: Theme.primaryColor
	text: propertyValue ? propertyValue.TypedValue.DisplayValue : ''
	validator: RealValidator {
		decimals: 2
		notation: "StandardNotation"
	}
}
