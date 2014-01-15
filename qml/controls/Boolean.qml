import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Boolean.js" as Logic

ComboBox {

	id: comboBox

	label: ''
	valueColor: Theme.primaryColor

	// Select the value.
	currentIndex: propertyValue.TypedValue.HasValue ? ( propertyValue.TypedValue.Value ? 1 : 2 ) : 0

	menu: ContextMenu {

		MenuItem { text: qsTr( "" ) }
		MenuItem { text: qsTr( "Yes" ) }
		MenuItem { text: qsTr( "No" ) }
	}
}
