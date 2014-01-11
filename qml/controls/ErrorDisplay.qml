import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

Overlay {

	id: errorDisplay
	overlayAlpha: 0.2
	z: 1 // Display on top of everything else.

	property AppMonitor monitor
	property variant detailsPage

	// Visible only if there are errors to display.
	visible: monitor.lastError.message !== '' && !detailsPage

	Overlay {

		id: messageArea

		overlayAlpha: 0.4

		// Position.
		fillScreen: false
		anchors.left: parent.left
		anchors.right: parent.right
		height: Theme.itemSizeLarge * 2
		y: (parent.height / 2) - Theme.itemSizeLarge

		BackgroundItem {

			// Position.
			anchors.fill: parent

			Column {

				// Positioning.
				anchors.fill: parent
				anchors.leftMargin: Theme.paddingLarge
				anchors.rightMargin: Theme.paddingLarge

				// Display the error.
				Label {

					id: message

					// Positioning.
					anchors.left: parent.left
					anchors.right: parent.right
					height: Theme.itemSizeMedium

					// Display.
					text: monitor.lastError.message
					verticalAlignment: Text.AlignVCenter
					//color: highlighted ? Theme.highlightColor : Theme.primaryColor
				}

				Button {

					id: details

					// Positioning.
					anchors.left: parent.left
					anchors.right: parent.right

					text: "Details"

					// Show our details page.
					onClicked: {
						detailsPage = pageStack.push(
									'../pages/ErrorDetails.qml', {
										error: monitor.lastError
									})
					}
				}
			}

			// The user can acknowledge the errors by clicking on them.
			onClicked: {
				monitor.hideErrors()
			}
		}
	}

	// Cleart the details page when the current page of the page stack changes. e.g our details page has been replaced.
	Connections {
		target: pageStack
		onCurrentPageChanged: {
			if (detailsPage && detailsPage !== pageStack.currentPage)
				detailsPage = null
		}
	}
}
