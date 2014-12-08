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
	property alias blockedLookups: valueListItemListModel.blockedLookups
	property alias selectedLookup: valueListItemListModel.selectedLookup

	DialogHeader {

		id: header

		acceptText: valueListItemListModel.selectedLookup ? valueListItemListModel.selectedLookup.DisplayValue : ''
	}

	SilicaListView {

		id: lookupListView

		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: Theme.paddingLarge

		model: ValueListItemListModel {

			id: valueListItemListModel

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
			highlighted: down || ( valueListItemListModel.selectedLookup ? model.id === valueListItemListModel.selectedLookup.Item : false )
			height: Theme.itemSizeExtraSmall
			enabled: model.selectable

			Label {

				// Position.
				anchors.fill: parent

				verticalAlignment: Text.AlignVCenter
				text: model.display
				color: model.selectable ? Theme.primaryColor : Theme.secondaryColor
			}

			// Select the clicked lookup
			onClicked: valueListItemListModel.selectedLookup = model.lookup
		}
	}
}
