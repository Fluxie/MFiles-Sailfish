import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils

Page {

	id: lookupListing

	property string title
	property VaultFront vault
	property alias propertyValue: lookupListModel.propertyValue

	allowedOrientations: Orientation.All

	// Declare list view that displays the property values.
	SilicaListView {

		id: lookupList

		// Position
		anchors.fill: parent

		// Header for the page.
		header: PageHeader {

			id: header
			title: lookupListing.title
		}

		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.leftMargin: Theme.paddingLarge
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		model: LookupListModel {
			id: lookupListModel
		}

		// Placeholder for empty list view.
		ViewPlaceholder {
			enabled: lookupList.count === 0
			text: lookupListing.title
		}

		// Delegate function that generates the list items.
		delegate: ListItem	{

			id: lookupDelegate
			contentHeight: Theme.itemSizeExtraSmall

			Label {

				id: label

				// Position.
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter

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
				var dialog = pageStack.push("../dialogs/SelectLookups.qml", {
												valueList: valueListId,
												propertyDefinition: propertyValue.PropertyDef,
												vault: vault,
												blockedLookups: blockedLookups,
												selectedLookup: model.lookup
											})
				dialog.accepted.connect(function () {

					// Replace the lookup with the one that was selected in the dialog.
					model.lookup = dialog.selectedLookup
				} );
			}

		}
	}
}
