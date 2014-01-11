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

	DialogHeader {

		id: header

		title: "Choose"
	}

	SilicaListView {

		id: lookupListView

		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: Theme.paddingLarge

		model: ValueListModel {
			valueList: valueListData ? valueListData : null
		}

		// Position
		header: SearchField {

			id: search

			anchors.left: parent.left
			anchors.right: parent.right

			placeholderText: 'Search'
		}

		ViewPlaceholder {

			id: placeholder
			text: 'Searching...'
		}

		delegate: ListItem {

			id: listItem

			Row {

				Label {

					verticalAlignment: Text.AlignVCenter
					text: model.display
				}
			}
		}
	}
}
