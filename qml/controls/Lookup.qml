import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Lookup.js" as Logic


Column {

	id: lookup

	// Properties
	property int propertyDefinitionId: propertyValue ? propertyValue.PropertyDef : -1
	property alias label: propertyLabel.label

	// Position
	anchors {
		fill: parent
		leftMargin: Theme.paddingLarge;
	}

	BackgroundItem {

		id: lookupSelector

		// Position.
		anchors {
			left: parent.left
			right: parent.right
		}

		// Do not enable before the property definition is available
		enabled: vault ? vault.propertyDefinitionsReady : false

		Label {

			id: lookupName

			height: Theme.itemSizeExtraSmall

			// Position.
			anchors {
				fill: parent
			}

			// Text.
			verticalAlignment: Text.AlignVCenter
			text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
		}



		// Action
		onClicked: {
			var propertyDefinition = vault.get( VaultFront.PropertyDefinition, propertyDefinitionId )
			var valueListId = propertyDefinition.ValueList
			var dialog = pageStack.push("../dialogs/SelectLookups.qml", {
											valueList: valueListId,
											filter: typedValue.filter,
											vault: vault,
											selectedLookup: propertyValue.TypedValue.Lookup
										})
			dialog.accepted.connect(function () {

				// Submit the selected lookup.
				typedValue.submit( dialog.selectedLookup );
			} );
		}
	}

	PropertyLabel {

		id: propertyLabel

		// Position.
		anchors {
			left: parent.left
			right: parent.right
		}
	}
}


