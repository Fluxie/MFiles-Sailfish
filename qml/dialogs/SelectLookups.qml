import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "../common/utils.js" as Utils

Dialog {

	id: lookupSelector

	property int valueList
	property TypedValueFilter filter
	property VaultFront vault
	property variant valueListData: vault && filter ? vault.valueList( valueList, filter ) : null
	property alias blockedLookups: valueListModel.blockedLookups
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
			highlighted: down || ( valueListModel.selectedLookup ? model.id === valueListModel.selectedLookup.Item : false )
			height: Theme.itemSizeExtraSmall

			Label {

				// Position.
				anchors.fill: parent

				verticalAlignment: Text.AlignVCenter
				text: model.display
			}

			// Select the clicked lookup
			onClicked: valueListModel.selectedLookup = model.lookup
		}
	}
}
