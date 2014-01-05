import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

TextField {

    property Loader container
	property variant value
	onValueChanged: {
		if( value.HasValue )
			text = value.Value;
		else
			text = ''
	}
	validator: IntegerValidator {}
}
