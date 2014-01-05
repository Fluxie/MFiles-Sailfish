import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

Page {

    id: errorDetails

    property ErrorModel error

    // Declare list view that displays the property values.
    SilicaListView {

        id: errorStack

        // Position
        anchors.fill: parent

        model: error.stack

        // Header for the page.
        header: PageHeader {

            id: header
            title: error.message
        }

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingLarge
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // Placeholder for empty list view.
        ViewPlaceholder {
            enabled: errorStack.count === 0
            text: error.message
        }

        // Delegate function that generates the list items.
        delegate: ListItem	{

            id: stackItem
            contentHeight: Theme.itemSizeExtraSmall

            Label {

                id: label

                // Position.
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                // Content
                text: model.display
                font.pixelSize: Theme.fontSizeExtraSmall
            }

        }
    }

}
