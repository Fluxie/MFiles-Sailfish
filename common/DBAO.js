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

// Helper function for brief joins
var _j = function( arr ) { return arr.join(' '); }

// Database upgrade functions
// There's no specific DB version variable. The version corresponds to the
// amount of upgrade functions executed.
// Just add more upgrade functions at the end to upgrade the DB.
var upgradeFunctions = [
	function( tx ) {

		tx.executeSql(
			"CREATE TABLE settings( name TEXT, value TEXT )" );

		tx.executeSql( _j([
			"CREATE TABLE previousVaults(",
				"id INTEGER PRIMARY KEY,",
				"name TEXT,",
				"guid TEXT,",
				"url TEXT",
				"username TEXT,",
				"authentication TEXT"
			")" ]));
	}
]

/**
 * Open the database
 *
 * @param {function} done Callback function for the database connection
 */
function _getDatabase( done ) {
	return openDatabaseSync( "MFiles", "0.1", "MFilesDB", 100 * 1024, done );
}


/**
 * Initialize the database
 *
 * @param {function} done Callback for when the database is initailzied
 */
function initialize( done ) {

	// Retrieve the database
	_getDatabase(function( db ) {

		// Read the DB version
		var dbVersion = 0;
		db.transaction( function( tx ) {

			tx.executeSql( "CREATE TABLE IF NOT EXISTS dbVersion( version INT )" );
			var results = tx.executeSql( "SELECT version FROM dbVersion" );

			if( results.length === 0 ) {
				tx.executeSql( "INSERT INTO version VALUES(0)" );
			} else {
				dbVersion = results.item(0).version;
			}
		});

		// Perform updates as long as the version is less than the amount of
		// upgrades available.
		while( dbVersion < upgradeFunctions.length ) {
			db.transaction( function( tx ) {
				upgradeFunctions[ dbVersion ]( tx );
				tx.executeSql( "UPDATE dbVersion SET version = ?", dbVersion + 1 );
			});

			dbVersion++;
		}

		done();
	});
};

/**
 * Retrieves a list of previous vaults
 *
 * @param {function} done Callback for the vault list
 */
function getPreviousVaults( done ) {
	_getDatabase( function( db ) {
		var results;
		db.transaction( function( tx ) {
			results = tx.executeSql( "SELECT id, name, guid, url, username, authentication FROM previousVaults" );
		});
		
		// Copy the SQL result objects into an array of plain objects.
		var arr = [];
		for( var i = 0; i < results.length; i++ ) {
			var result = results.item(i);
			arr.push({
				id: result.id,
				name: result.name,
				guid: result.guid,
				url: result.url,
				username: result.username,
				authentication: result.authentication
			});
		}
	});
};

/**
 * Saves a vault in the list of previous vaults
 *
 * @param {object} vault Vault information
 * @param {function} [done] Callback for when the save is finished
 */
function savePreviousVault( vault, done ) {
	_getDatabase( function( db ) {

		db.transaction( function( tx ) {

			tx.executeSql(
				"INSERT INTO previousVaults( name, guid, url, username, authentication ) " +
				"VALUES ( ?, ?, ?, ?, ? )",
				vault.name, vault.guid, vault.url, vault.username, vault.authentication );
		});

		if( done ) { done(); }
	});
};

