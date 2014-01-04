import QtQuick 2.0

// A full screen overlay.
Rectangle {

    id: errorDisplay

    property bool fillScreen: true
    property color overlayColor: Qt.rgba( 0, 0, 0, 1.0 )
    property real overlayAlpha: 1.0
    property point rootOrigo: Qt.point( 0, 0 )

    // Positioning
    Component.onCompleted: {

        // Convert the coordinates of this item to the root item's coordinate system and
        // then use those coordinates. This positions the overlay so that it will cover the entire page.
        var point = mapFromItem( null, 0, 0 );
        rootOrigo.x = point.x;
        rootOrigo.y = point.y;
    }
    x: fillScreen ? rootOrigo.x : 0  // If we want to fill the screen, we need to use coordinates scaled to the root item.
    y: fillScreen ? rootOrigo.y : 0  // If we want to fill the screen, we need to use coordinates scaled to the root item.
    z: 1  // Display on top of everything else.
    height: fillScreen ? screen.height : childrenRect.height
    width: fillScreen ? screen.width : childrenRect.width
    color: Qt.rgba( overlayColor.redF, overlayColor.greenF, overlayColor.blueF, overlayAlpha )

}
