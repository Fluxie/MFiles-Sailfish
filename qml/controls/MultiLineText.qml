import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

TextArea {

    // Properties
    property variant propertyValue
    property VaultFront vault

    text: propertyValue ? propertyValue.TypedValue.DisplayValue : ''
}
