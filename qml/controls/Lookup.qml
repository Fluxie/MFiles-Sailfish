import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Lookup.js" as Logic

BackgroundItem {

	id: lookup

	// Properties
	property int propertyDefinitionId: propertyValue ? propertyValue.PropertyDef : -1

	// Do not enable before the property definition is available
	enabled: vault ? vault.propertyDefinitionsReady : false

	Label {

		// Position.
		anchors.fill: parent
		anchors.leftMargin: Theme.paddingLarge

		// Text.
		verticalAlignment: Text.AlignVCenter
		text: propertyValue ? propertyValue.TypedValue.DisplayValue : ""
	}

	// Action
	onClicked: {
		console.log("onClicked")
		console.log("Property definition:" + propertyDefinitionId)
		var propertyDefinition = vault.get( VaultFront.PropertyDefinition, propertyDefinitionId )
		var valueListId = propertyDefinition.ValueList
		var dialog = pageStack.push("../dialogs/SelectLookups.qml", {
										valueList: valueListId,
										propertyDefinition: propertyDefinitionId,
										vault: lookup.vault
									})
		dialog.accepted.connect(function () {})
	}
}
