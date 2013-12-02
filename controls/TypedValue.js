
.import Sailfish.Silica 1.0 as Silica

var __TVMFDataTypes = {

	Text : 1,
	Integer : 2,
	Floating : 3,
	Date : 5,
	Time : 6,
	Timestamp : 7,
	Boolean : 8,
	Lookup : 9,
	MultiLineText : 13,
	MultiSelectLookup : 10,
}

/**
 * Selects the control for displaying the typed value.
 *
 * @param {typedValue} Typed value
 */
function selectControl( typedValue ) {

	// Select the control based on the data type.
	switch( typedValue.DataType ) {

	// Single-line text
	case 1:
		return "SingleLineText.qml";

	// Integer
	case 2:
		return "Integer.qml";

	// Real
	case 3:
		return "Real.qml";

	// Date
	case 5:
		return "Date.qml";

	// Time
	case 6:
		return "Time.qml";

	// Boolean
	case 8:
		return "Boolean.qml";

	// Multi-select lookup
	case 10:
		return "Lookups.qml";

	case 13:
		return "MultiLineText.qml";

	// We return simple read-only control for types we do not recognize.
	default:
		return "ReadOnlyLabel.qml";
	}
}

/**
 * Selects implicit height for the typed value control.
 *
 * @param {typedValue} Typed value
 */
function selectImplicitHeight( typedValue ) {

	// Implicit height based on the data type.
	switch( typedValue.DataType ) {

	// Single-line text
	case 1:
		return Silica.Theme.itemSizeExtraSmall;

	// Multi-select lookup
	case 10:
		var lookups = typedValue.Lookups
		return selectImplicitHeightForMSLookup( lookups );

	case 13:
		return Silica.Theme.itemSizeExtraLarge * 2;

	// We return simple read-only control for types we do not recognize.
	default:
		return Silica.Theme.itemSizeExtraSmall;
	}

}


/**
 * Selects implicit height for the typed value control that represents multi-select lookup.
 *
 * @param {lookups} Lookups
 */
function selectImplicitHeightForMSLookup( lookups )
{

	var itemsShown = lookups.length;
	if( itemsShown > 2 )
		itemsShown = 3;
	return itemsShown * Silica.Theme.itemSizeExtraSmall;
}
