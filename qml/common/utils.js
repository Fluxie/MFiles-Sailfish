/*
 *  Copyright 2013 Mikko Rantanen
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

.pragma library

/**
 * Gets proper time format for formatting Time.
 *
 * @returns {object} Time format.
 */
function getTimeFormat() {
	// Construct and return the time format.
	var timeFormat = Qt.locale().timeFormat( Qt.DefaultLocaleLongDate ).replace( /[.]/g, ":" ).replace( /t/g, "" ).replace( /^\s*/, "" ).replace( /^\s*$/, "" )
	return timeFormat;
}

/**
 * Create a deep copy of the object
 *
 * @param {object} obj Object to copy
 * @returns {object} Copy
 */
function deepCopy( obj ) {
	// Check the trivial cases.
	if( obj === null ) { return null; }
	if( obj === undefined ) { return undefined; }

	var c;

	// If this is a complex type, construct it properly.
	if( obj instanceof Array ) { c = []; }
	else if( obj instanceof Object ) { c = {}; }

	// c is initialized now only if it should be a complex type.
	// Both of these are constructed in the same way through a for-loop.
	if( c !== undefined ) {
		for( var i in obj ) { c[i] = deepCopy( obj[i] ); }
		return c;
	}

	// Object isn't a complex type. No need for a copy.
	return obj;
};

/**
 * Merge missing properties from the defaults to the options
 *
 * @param {object} options Options to merge to
 * @param {objec} defaults Options to merge from
 */
function merge( options, defaults ) {
	for( var d in defaults ) {
		if( options[d] === undefined ) {
			options[d] = defaults[d];
		}
	}

	return options;
};


/**
 * Allows inspecting the paremeter with debugger
 *
 * @param {inspected} object for inspection
 */
function inspect( inspected )
{
	return inspected;
}
