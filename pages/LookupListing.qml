import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

	id: lookupListing

	property string title
	property variant typedValue
	property variant lookupValues

	onTypedValueChanged: {
		lookupValues = typedValue.Lookups
		lookupList.lookupCount = lookupValues.length
	}

	// Declare list view that displays the property values.
	SilicaListView {

		id: lookupList

		// Position
		anchors.fill: parent

		// The number of property values.
		// We use the number of property values as the model and update the model here.
		property int lookupCount: 0
		onLookupCountChanged: {
			lookupList.model = lookupCount;
		}

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
				text: lookupListing.lookupValues[ index ].DisplayValue
			}

		}
	}
}
