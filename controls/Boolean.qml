import QtQuick 2.0
import Sailfish.Silica 1.0

ComboBox {

	label: ''

	property bool dynamicHeight: true
	property variant value
	onValueChanged: {
		if( ! value.HasValue )
			currentIndex = 0;
		else if( value.Value )
			currentIndex = 1;
		else
			currentIndex = 2;
	}

	menu: ContextMenu {

		MenuItem { text: "" }
		MenuItem { text: "Yes" }
		MenuItem { text: "No" }
	}
}
