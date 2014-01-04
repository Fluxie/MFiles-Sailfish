import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

Overlay {

    id: errorDisplay
    overlayAlpha: 0.2

    property VaultFront vault

    // Visible only if there are errors to display.
    visible: vault.monitor.lastError !== ''

    Overlay {

        id: messageArea

        overlayAlpha: 0.4

        // Position.
        fillScreen: false
        anchors.left: parent.left
        anchors.right: parent.right
        height: Theme.itemSizeMedium
        y: ( parent.height / 2 ) - Theme.itemSizeLarge

        BackgroundItem {

            // Position.
            anchors.fill: parent

            // Display the error.
            Label {

                id: message

                // Positioning.
                anchors.fill: parent
                anchors.leftMargin: Theme.paddingLarge
                anchors.rightMargin: Theme.paddingLarge

                // Display.
                text: vault.monitor.lastError
                verticalAlignment: Text.AlignVCenter
                //color: highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            // The user can acknowledge the errors by clicking on them.
            onClicked: {
                vault.monitor.hideErrors();
            }
        }

    }
}

