import QtQuick 2.0
import Sailfish.Silica 1.0

import "Boolean.js" as Logic

ComboBox {

    id: comboBox

	label: ''

    property Loader container
	property bool dynamicHeight: true
    property variant value

    Connections {
        target: comboBox.container
        ignoreUnknownSignals: true
        onPropertyValueChanged: {
            Logic.setValue( comboBox.container, comboBox );
        }
        onTargetChanged: {
            Logic.setValue( comboBox.container, comboBox );
        }
    }

	menu: ContextMenu {

		MenuItem { text: "" }
		MenuItem { text: "Yes" }
		MenuItem { text: "No" }
	}
}
