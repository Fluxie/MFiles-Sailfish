/*
 *  Copyright 2013 Juha Lepola
 *
 *  This file is part of M-Files for Sailfish.
 *
 *  M-Files for Sailfish is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  M-Files for Sailfish is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with M-Files for Sailfish. If not, see
 *  <http://www.gnu.org/licenses/>.
 */

.import "../common/utils.js" as Utils
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

	// Single-select lookup
	case 9:
		return "Lookup.qml";

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
	var itemHeight;
	switch( typedValue.DataType ) {

	// Single-line text
	case 1:
		itemHeight = Silica.Theme.itemSizeExtraSmall;
		break;

	// Multi-select lookup
	case 10:
		var lookups = typedValue.Lookups
		itemHeight = selectImplicitHeightForMSLookup( lookups );
		break;

	// Multi-line text
	case 13:
		itemHeight = Silica.Theme.itemSizeExtraLarge;
		break;

	// We return simple read-only control for types we do not recognize.
	default:
		itemHeight = Silica.Theme.itemSizeExtraSmall;
		break;
	}

	// Include the size of the label.
	var totalHeight = itemHeight + Silica.Theme.itemSizeExtraSmall;
	return totalHeight;
}

/**
 * Creates a new typed value based on an existing property value with an updated data.
 *
 * @param {value] typedValue The typed value used as a base.
 * @param {value} value Updated data.
 */
function setTypedValue( typedValue, value ) {

	// Update the data.
	switch( typedValue.DataType )
	{
	// Text
	case 1:
	case 13:
		typedValue.Value = value.toString();
		typedValue.DisplayValue = value.toString();
		break;

	// Integer
	case 2 :
		typedValue.Value = value;
		typedValue.DisplayValue = value.toString();
		break;

	// Real
	case 3 :
		typedValue.Value = value;
		typedValue.DisplayValue = Number( value ).toLocaleString( Qt.locale(),  "f", 2 );
		break;

	// Date
	case 5 :
		typedValue.Value = value;
		typedValue.DisplayValue = Qt.formatDate( value, Qt.DefaultLocaleShortDate );
		break;

	// Time
	case 6 :
		typedValue.Value = value;
		var timeForDisplay = new Date();
		timeForDisplay.setHours( value.getUTCHours() );
		timeForDisplay.setMinutes( value.getUTCMinutes() );
		timeForDisplay.setSeconds( value.getUTCSeconds() );
		typedValue.DisplayValue = timeForDisplay.toLocaleTimeString( Qt.locale(), Utils.getTimeFormat() );
		break;

	// Single-select lookup
	case 9 :
		typedValue.Value = value;
		typedValue.Lookup = value;
		typedValue.DisplayValue = value.DisplayValue;
		break;

	// Multi-select lookup
	case 10 :
		typedValue.Value = value;
		typedValue.Lookups = value;
		break;

	// For now, no-op. TODO Make this an error.
	default:
		console.log( typedValue );
		console.trace();
		break;

	}

	// Update the HasValue filed.
	typedValue.HasValue = typedValue.Value ? true : false;

	// Print the value.
	// Utils.trace( typedValue );
}

/**
 * Selects implicit height for the typed value control that represents multi-select lookup.
 *
 * @param {lookups} Lookups
 */
function selectImplicitHeightForMSLookup( lookups )
{
	// Exit if empty.
	if( ! lookups || lookups.length === 0 )
		return Silica.Theme.itemSizeExtraSmall * 2;

	var itemsShown = lookups.length;
	if( itemsShown > 2 )
		itemsShown = 3;
	var itemHeight = itemsShown * Silica.Theme.itemSizeExtraSmall;
	return itemHeight;
}
