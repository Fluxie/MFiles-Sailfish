import QtQuick 2.0
import Sailfish.Silica 1.0

SilicaFlickable {

	id: extendedText	
	anchors.top: parent.top
	//anchors.bottom: parent.bottom
	anchors.left: parent.left
	anchors.right: parent.right
	height: field.implicitHeight + Theme.paddingSmall
	contentHeight: field.implicitHeight
	contentWidth: field.implicitWidth;
	flickableDirection: Flickable.HorizontalFlick

    property Loader container
	property variant value
	onValueChanged: {
		if( value )
			field.text = value.DisplayValue;
	}
	Label {

		id: field
		anchors.fill: parent


		text: ''
	}

	HorizontalScrollDecorator { flickable: extendedText }
}
