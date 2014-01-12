import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {

	id: line

	width: 2

	// Line gradient.
	gradient: Gradient {
			GradientStop { position: 0.0; color: Theme.rgba( line.color, 0.5 ) }
			GradientStop { position: 1.0; color:  Theme.rgba( line.color, 0.0 ) }
	}

}
