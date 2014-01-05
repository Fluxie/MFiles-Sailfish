import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0
import "controls"
import "pages"

ApplicationWindow
{
    initialPage: LogIn { }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    // Define error display.
    ErrorDisplay {

        id: errorDisplay

        monitor: GlobalMonitor
    }

}
