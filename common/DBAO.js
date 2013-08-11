/*
 *  Copyright 2013 Mikko Rantanen
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
				"url TEXT,",
				"username TEXT,",
				"authentication TEXT",
			")" ]));
	}
]

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

        tx.executeSql( "CREATE TABLE IF NOT EXISTS dbVersion( version INT )" );
        var results = tx.executeSql( "SELECT version FROM dbVersion" );

        if( results.rows.length === 0 ) {
            tx.executeSql( "INSERT INTO dbVersion VALUES(0)" );
        } else {
            dbVersion = results.rows.item(0).version;
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
};

/**
 * Retrieves a list of previous vaults
 *
 * @returns {Vault[]} Array of vaults
 */
function getPreviousVaults() {
	var db = _getDatabase();

    var results;
    db.transaction( function( tx ) {
        results = tx.executeSql( "SELECT id, name, guid, url, username, authentication FROM previousVaults ORDER BY name" );
    });
    
    // Copy the SQL result objects into an array of plain objects.
    var arr = [];
    for( var i = 0; i < results.rows.length; i++ ) {
        var result = results.rows.item(i);
        arr.push({
            id: result.id,
            name: result.name,
            guid: result.guid,
            url: result.url,
            username: result.username,
            authentication: result.authentication
        });
    }

    return arr;
};

/**
 * Saves a vault in the list of previous vaults
 *
 * @param {object} vault Vault information
 */
function savePreviousVault( vault, rememberPassword ) {
	var db = _getDatabase();

    // If we shouldn't remember password, store authentication as null.
    var auth = rememberPassword ? vault.authentication : null;
    console.log( "DBAO, auth: " + auth );
    console.log( "DBAO, authentication: " + vault.authentication );

    db.transaction( function( tx ) {

        if( vault.id === null || vault.id === undefined ) {
            console.log( "New vault. Creating new" );
            tx.executeSql(
                "INSERT INTO previousVaults( name, guid, url, username, authentication ) " +
                "VALUES ( ?, ?, ?, ?, ? )",
                [ vault.name, vault.guid, vault.url, vault.username, auth ]);
        } else {
            console.log( "Saving over old one. Authentication:" + auth );
            tx.executeSql(
                "UPDATE previousVaults SET name=?, username=?, authentication=? WHERE id=?",
                [ vault.name, vault.username, auth, vault.id ]);
        }
    });
    console.log( "Done saving ");
};

