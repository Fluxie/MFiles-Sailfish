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
  * Traces the contents of JSON object.
  */
function trace( obj ) {

	// Check the trivial cases.
	if( obj === null ) { console.log( "null" ); return null; }
	if( obj === undefined ) { console.log( "undefined" ); return undefined; }

	// Print based on the type.
	if( obj instanceof Array ||
		obj instanceof Object )
	{
		for( var field in obj ) { trace( field ); }
	}
	else
	{
		// Simple type.
		console.log( obj );
	}
};

/**
 * Merge missing properties from the defaults to the options
 *
 * @param {object} options Options to merge to
 * @param {object} defaults Options to merge from
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
  * Returns an array of objects given all expect the one listed in except.
  *
  * @param {object} all Array of objects
  * @param {object} except Object that is excluded from the return.
  * @param {function} getId Function that returns the id of the object.
  */
function except( all, except, getId ) {

	// Iterate over the items in 'all' and collect all items except the one specified in except.
	var exceptId = getId( except );
	var otherCount = all.length - 1;
	var others = new Array( 0 );
	if( otherCount > 0 )
	{
		// Collect the objects that are included all excepte the value specified in 'except.
		var id;
		all.forEach( function( other ) {

			id = getId( other );
			if( id !== exceptId )
				others.push( other );
		} );

	}  // end if

	// The others.
	return others;
}


/**
 * Allows inspecting the paremeter with debugger
 *
 * @param {inspected} object for inspection
 */
function inspect( inspected )
{
	return inspected;
}
