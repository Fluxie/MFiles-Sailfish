/*
 *  Copyright 2015 Mikko Rantanen, Juha Lepola
 *
 *  This file is part of M-Files for Sailfish.
 *
 *  M-Files for Sailfish is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License.
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

.import QtQuick.LocalStorage 2.0 as LS
.import 'structs.js' as Structs

// Helper function for brief joins
var _j = function( arr ) { return arr.join(' '); }

// Database upgrade functions
// There's no specific DB version variable. The version corresponds to the
// amount of upgrade functions executed.
// Just add more upgrade functions at the end to upgrade the DB.
var upgradeFunctions = [
	function( tx ) {

		// Storage for settings.
		tx.executeSql(
			"CREATE TABLE Settings( name TEXT, value TEXT )" );

		// Storage for vaults.
		tx.executeSql( _j([
			"CREATE TABLE Vaults(",
				"id INTEGER PRIMARY KEY,",
				"name TEXT,",
				"guid TEXT,",
				"url TEXT,",
				"username TEXT,",
				"password TEXT",
				")" ]));
	}
]

// List of all possible tables so we can reset the DB easily
var _tables = [ 'DbVersion', 'Settings', 'Vaults' ];

/**
 * Open the database
 */
function _getDatabase() {
	var db = LS.LocalStorage.openDatabaseSync( "MFiles", "1.0", "MFilesDB", 100 * 1024 );
	return db;
}


/**
 * Initialize the database
 */
function initialize() {

	// Retrieve the database
	var db = _getDatabase();

	// Read the DB version
	var dbVersion = 0;
	db.transaction( function( tx ) {

		tx.executeSql( "CREATE TABLE IF NOT EXISTS DbVersion( version INT )" );
		var results = tx.executeSql( "SELECT version FROM DbVersion" );

		if( results.rows.length === 0 ) {
			tx.executeSql( "INSERT INTO DbVersion VALUES( 1 )" );
		} else {
			dbVersion = results.rows.item(0).version;
		}
	});

	// Perform updates as long as the version is less than the amount of
	// upgrades available.
	while( dbVersion < upgradeFunctions.length ) {
		db.transaction( function( tx ) {
			upgradeFunctions[ dbVersion ]( tx );
			tx.executeSql( "UPDATE DbVersion SET version = ?", dbVersion + 1 );
		});

		dbVersion++;
	}
};

/**
 * Drops all tables from the database.
 */
function dropDatabase() {
	var db = _getDatabase();

	db.transaction( function( tx ) {
		// Note: Not using SQL parameters here.
		// _tables should be safe from injection attacks.
		// Not sure if DROP TABLE even eccepts them.
		for( var t in _tables ) {
			var table = _tables[t];
			tx.executeSql( "DROP TABLE IF EXISTS " + table );
		}
	});
}

/**
 * Retrieves a list of previous vaults
 *
 * @returns {Vault[]} Array of vaults
 */
function getRegisteredVaults() {
	var db = _getDatabase();

	var results;
	db.transaction( function( tx ) {
		results = tx.executeSql( "SELECT id, name, guid, url, username, pasword FROM Vaults ORDER BY name" );
	});

	// Copy the SQL result objects into an array of plain objects.
	var arr = [];
	for( var i = 0; i < results.rows.length; i++ ) {
		var result = results.rows.item(i);
		arr.push( new Structs.Vault( result ) );
	}

	return arr;
};

/**
 * Saves a vault in the vault list.
 *
 * @param {object} vault Vault information
 */
function saveVault( vault ) {
	var db = _getDatabase();

	db.transaction( function( tx ) {

		// New or existing?
		if( vault.id === null || vault.id === undefined ) {
			var insertResult = tx.executeSql(
				"INSERT INTO Vaults( name, guid, url, username, password ) " +
				"VALUES ( ?, ?, ?, ?, ? )",
				[ vault.name, vault.guid, vault.url, vault.username, vault.password ]);

			vault.id = Number( insertResult.insertId );
		} else {
			tx.executeSql(
				"UPDATE previousVaults SET name=?, username=?, password=? WHERE id=?",
				[ vault.name, vault.username, vault.password, vault.id ]);
		}
	});
};

