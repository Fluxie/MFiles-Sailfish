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
