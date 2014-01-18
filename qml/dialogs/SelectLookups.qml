import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils

Dialog {

	id: lookupSelector

	property int valueList
	property int propertyDefinition
	property VaultFront vault
	property variant valueListData: vault ? vault.valueList( valueList, propertyDefinition ) : null
	property alias selectedLookup: valueListModel.selectedLookup

	DialogHeader {

		id: header

		acceptText: valueListModel.selectedLookup ? valueListModel.selectedLookup.DisplayValue : ''
	}

	SilicaListView {

		id: lookupListView

		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: Theme.paddingLarge

		model: ValueListModel {

			id: valueListModel

			valueList: valueListData ? valueListData : null
		}

		// Position
		header: SearchField {

			id: search

			placeholderText: 'Search'
		}

		ViewPlaceholder {

			id: placeholder
			text: 'Searching...'
		}

		delegate: ListItem {

			id: listItem

			// Set highlight if presser or if selected.
			highlighted: down || model.id === valueListModel.selectedLookup.Item

			Row {

				anchors.fill: parent

				Label {

					// Position.
					anchors { top: parent.top; bottom: parent.bottom }

					verticalAlignment: Text.AlignVCenter
					text: model.display
				}
			}

			// Select the clicked lookup
			onClicked: valueListModel.selectedLookup = model.lookup
		}
	}
}
