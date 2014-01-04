import QtQuick 2.0
import Sailfish.Silica 1.0


TextField {

	id: field	
	color: Theme.primaryColor

    property Loader container
	property variant value

	text: value ? value.DisplayValue : ''


}


