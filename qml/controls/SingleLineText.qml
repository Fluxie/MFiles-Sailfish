import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

TextField {

	id: field

    // Properties
    property variant propertyValue
    property VaultFront vault

    // Text
    color: Theme.primaryColor
    text: propertyValue ? propertyValue.TypedValue.DisplayValue : ''
}


