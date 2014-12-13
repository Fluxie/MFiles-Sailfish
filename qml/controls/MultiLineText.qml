import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0
import "../visuals"

Column {

	id: multilineText

	// Internal properties
	property bool _extended: false
	property int _maximumHeightAdjustment: _extended ? 4 : 2
	property alias label: textEditor.label

	anchors {
		leftMargin: Theme.paddingLarge;
	}

	// The actual control for editing the text.
	Row {

		// A little room for the controls.
		spacing: 2

		// Position the row.
		anchors.left: parent.left
		anchors.right: parent.right

		// The height equals the dynamic height, but is then capped.
		height: Math.min( Theme.itemSizeExtraLarge * _maximumHeightAdjustment, textEditor.implicitHeight )

		// The actual editor area.
		TextArea {

			id: textEditor

			// Position to bottom.
			anchors.bottom: parent.bottom

			// The height equals the dynamic height, but is then capped.
			height: Math.min( Theme.itemSizeExtraLarge * _maximumHeightAdjustment, implicitHeight )
			width: multilineText.width - 20

			// Text.
			color: Theme.primaryColor
			text: propertyValue ? propertyValue.TypedValue.DisplayValue : ''
			placeholderText: textEditor.label

			// Submit the changes when the active focus is lost.
			onActiveFocusChanged: {
				if( ! activeFocus )
					typedValue.submit( textEditor.text );
			}
		}

		// A vertical line on the right side of the text editor control.
		// Shown if the editor area becomes very large.
		Line {

			id: line

			// Show the line only if the control is large enough.
			visible: showMore.visible

			// Position.
			anchors.top: parent.top
			anchors.bottom: parent.bottom

			// Color.
			color: Theme.primaryColor
		}
	}

	// Button for displaying larger area.
	Button {

		id: showMore

		// Position below the separator
		anchors.horizontalCenter: parent.horizontalCenter

		// The button will be visible only if there is enough text to warrant the "more" action.
		visible: textEditor.implicitHeight >= Theme.itemSizeExtraLarge * 2

		// Button label
		text: _extended ? qsTr( "Less") : qsTr( "More" )

		// Toggle the extended state.
		onClicked: _extended = !_extended
	}
}
