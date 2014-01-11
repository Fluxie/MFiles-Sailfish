import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Boolean.js" as Logic

ComboBox {

    id: comboBox

	label: ''
    valueColor: Theme.primaryColor

    // Properties
    property variant propertyValue
    property VaultFront vault
	property bool dynamicHeight: true
    onPropertyValueChanged: {
        Logic.setValue( propertyValue, comboBox );
    }

	menu: ContextMenu {

        MenuItem { text: qsTr( "" ) }
        MenuItem { text: qsTr( "Yes" ) }
        MenuItem { text: qsTr( "No" ) }
	}
}
