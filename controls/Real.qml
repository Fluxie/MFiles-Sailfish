import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

TextField {

	property variant value
	onValueChanged: {
		if( value.HasValue )
			text = value.DisplayValue;
		else
			text = ''
	}
	validator: RealValidator { decimals: 2; notation: "StandardNotation" }
}
