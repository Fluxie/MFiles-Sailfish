import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils


// Typed value is represented by a loader as we will dynamically select the control we use to display the value.
Column {

	id: lookups

	// Position
	anchors.fill: parent

	// Lookup values
	Repeater {
		id: content

		model: LookupListModel {

			id: lookupListModel

			rowLimit: 2
			propertyValue: typedValue.propertyValue
		}

		BackgroundItem {

			// Position
			height: Theme.itemSizeExtraSmall

			Label {

				// Position.
				anchors.fill: parent
				anchors.leftMargin: Theme.paddingLarge

				// Content
				verticalAlignment: Text.AlignVCenter
				text: model.display
			}

			// Action
			onClicked: {

				// Block lookups that are already values of the property value.
				var blockedLookups = Utils.except(
							propertyValue.TypedValue.Lookups, model.lookup, function( lookup ) { return lookup.Item; });

				// Show the page.
				var propertyDefinition = vault.get( VaultFront.PropertyDefinition, propertyValue.PropertyDef )
				var valueListId = propertyDefinition.ValueList
				console.log( "About to display SelectLookups dialog.");
				var dialog = pageStack.push("../dialogs/SelectLookups.qml", {
												valueList: valueListId,
												propertyDefinition: propertyValue.PropertyDef,
												vault: vault,
												filter: typedValue.filter,
												blockedLookups: blockedLookups,
												selectedLookup: model.lookup
											})
				dialog.accepted.connect(function () {

					// Submit the updated lookups.
					model.lookup = dialog.selectedLookup
					typedValue.submit( lookupListModel.propertyValue.TypedValue.Lookups );
				} );
			}
		}
	}

	// Item that is visible if there are no lookups to display.
	BackgroundItem {

		// Position
		height: Theme.itemSizeExtraSmall

		// Show only if no lookups.
		visible: lookupListModel.lookupCount == 0

		// Action
		onClicked: {			

			// Show the page.
			var propertyDefinition = vault.get( VaultFront.PropertyDefinition, propertyValue.PropertyDef )
			var valueListId = propertyDefinition.ValueList
			var dialog = pageStack.push("../dialogs/SelectLookups.qml", {
											valueList: valueListId,
											propertyDefinition: propertyValue.PropertyDef,
											vault: vault,
											filter: typedValue.filter,
											blockedLookups: undefined,
											selectedLookup: undefined
										})
			dialog.accepted.connect(function () {

				// Submit the updated lookups.
				var lookups = new Array( 1 );
				lookups[ 0 ] = dialog.selectedLookup;
				typedValue.submit( lookups );
			} );
		}

	}

	// Button for opening lookup listing.
	Button {
		id: more

		// Position
		height: Theme.itemSizeExtraSmall
		anchors.horizontalCenter: parent.horizontalCenter

		// Visibility
		visible: lookupListModel.lookupCount > lookupListModel.rowLimit
		enabled: lookupListModel.lookupCount > lookupListModel.rowLimit

		// Content
		text: "More"

		// Action
		onClicked: {
			var listing = pageStack.push('../dialogs/LookupEditor.qml', {
											 title: propertyDefinitionName,
											 propertyValue: typedValue.propertyValue,											 
											 vault: typedValue.vault,
											 filter: typedValue.filter
										 })
			listing.accepted.connect( function() {

				// Submit the updated lookups.
				typedValue.submit( listing.propertyValue.TypedValue.Lookups );

			} );
		}
	}
}
