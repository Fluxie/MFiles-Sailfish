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

.import '../common/http.js' as Http
.import '../common/DBAO.js' as DBAO

/**
 * Initialize the login screen
 *
 * @param {ListModel} model Vaults list model
 * @param {Column} newVaultInfo Form for the new vault info
 * @param {Button} addNewVaultButton New vault button in the pull down menu
 */
function initialize( model, newVaultInfo, addNewVaultButton ) {
    DBAO.initialize();
    var vaults = DBAO.getPreviousVaults();

    if( vaults.length === 0 ) {
        addNewVaultButton.visible = false;
        newVaultInfo.visible = true;
    } else {
        for( var v in vaults ) {
            var vault = vaults[v];
            model.append(vault);
        }
    }
}

/**
 * Log ins to a new vault using supplied username and password
 *
 * @param {string} url MFWA url
 * @param {string} username Username
 * @param {string} password Password
 * @param {bool} rememberPassword True if auth token should be saved
 * @param {function} done Callback for error reporting
 */
function logInToNewVault( url, username, password, rememberPassword, done ) {

    // Log in to application and get list of vaults.
    _getVaults( url, username, password, function( err, vaults ) {
        if( err ) { return done( err ); }

        // Display the vault selection list
        _selectVault( vaults, function( err, vault ) {
            if( err ) { return done( err ); }

            // Fill the application level properties.
            vault.url = url;
            vault.username = username;

            // Make sure the selected vault is valid before navigating away
            _testVault( vault, function( err ) {
                if( err ) { return done( err ); }

                // Save the vault and proceed.
                _saveVault( vault, rememberPassword );
                _useVault( vault, true );
            });
        });
    });
}

/**
 * Logs into a previously saved vault
 *
 * @param {Vault} vault Vault record
 * @param {string} password Password typed by the user
 * @param {bool} rememberPassword true if password should be saved
 * @param {function} done Callback
 */
function logInToVault( vault, password, rememberPassword, done ) {


    console.log("FFFFUUUUUUUUUUUUUUUUUUUUUU: " + vault.authentication);
    console.log( typeof( vault.authentication ) );
    console.log( rememberPassword );

    // If password shouldn't be remembered, clear it from the vault.
    // This is done in case it was remembered previously.
    if( !rememberPassword ) { vault.authentication = null; }

    // If there are no saved authentication details,
    // attempt to log in with password.
    if( !vault.authentication ) {
        _getToken( vault.url, vault.username, password, vault.guid, function( err, token ) {
            if( err ) { return done( err ); }

            // Store the authenticationt oken so the main screen can use it.
            vault.authentication = token;
            console.log( "Token: " + token );
            console.log( "vault.auth: " + vault.authentication );
            
            // Login succeeded, save possible changes and proceed.
            console.log("Saving vault info. Remembering password? " + rememberPassword );
            _saveVault( vault, rememberPassword );
            console.log( "Displaying vault home ");
            _useVault( vault, false );
        });
    } else {
        
        // If the vault has saved authentication details use those.
        // Just make sure they are valid first.
        _testVault( vault, function( err ) {
            _useVault( vault, false );
        });
    }
}

/**
 * Display the vault select dialog and retrieve the selected vault
 *
 * @param {Vault[]} vaults Collection of vaults to select from
 * @param {function} done Callback to receive the vault
 */
function _selectVault( vaults, done ) {

    // Display the vault selection dialog
    var page = pageStack.push( "SelectVaultDialog.qml" );
    for( var v in vaults ) {
        page.listModel.append({
            name: vaults[v].Name,
            guid: vaults[v].GUID,
            authentication: vaults[v].Authentication
        });
    }

    // Once the vault is selected, signal the caller.
    // In this case the caller is responsible for popping
    // or replacing the dialog.
    page.accepted.connect( function() {
        done( null, {
            name: page.vaultName,
            guid: page.vaultGuid,
            authentication: page.vaultAuthentication
        });
    });

    // Pop the vault select dialog away on reject.
    page.rejected.connect( function() {
        pageStack.pop();
        done();
    });
};

/**
 * Performs login and retrieves the list of vaults
 *
 * @param {string} url M-Files URL
 * @param {string} username Username
 * @param {string} password Password
 * @param {function} done Receives the list of vaults
 */
function _getVaults( url, username, password, done ) {
    var client = new Http.HttpClient();
    client.headers[ 'X-Username' ] = username;
    client.headers[ 'X-Password' ] = password;

    client.get( url + '/REST/server/vaults', done );
}

/**
 * Saves the vault to the list of previous vaults
 *
 * @param {object} vault Vault information
 * @param {function} done Callback for when the save completes
 */
function _saveVault( vault, rememberPassword ) {
	DBAO.savePreviousVault( vault, rememberPassword );
}

function _getToken( url, username, password, guid, done ) {
	var client = new Http.HttpClient();
    client.headers[ 'X-Username' ] = username;
    client.headers[ 'X-Password' ] = password;
    client.headers[ 'X-Vault' ] = guid;
    client.get( url + '/REST/session/vault', function( err, vault ) {
        if( err ) { return done(err); }
        done( null, vault.Authentication );
    });

    // "Real" way of getting the token is to post to authentiontokens.
    // However this doesn't validate the token by performing login.
    // /session/vault should login.
    //
    // client.post( url + '/REST/server/authenticationtokens', {
    //     Username: username,
    //     Password: password,
    //     VaultGuid: guid
    // }, done );
}

/**
 * Tests whether the authentication details on the vault are valid
 *
 * @param {Vault} vault Vault with authentication information
 * @param {function} done Return callback
 */
function _testVault( vault, done ) {
	// Make sure the authentication details are still valid.
	var client = new Http.HttpClient();
	client.headers[ 'X-Authentication' ] = vault.authentication;
	client.get( vault.url + '/REST/session', function( err, sessionInfo ) {
        if( err ) { return done(err); }
        return done();
    });
};

/**
 * Chooses a vault and moves to the vault home
 *
 * @param {object} vault Vault information
 */
function _useVault( vault, replace ) {

    var dialog;
    if( replace ) {
        dialog = pageStack.replace( "VaultHome.qml" );
    } else {
        dialog = pageStack.push( "VaultHome.qml" );
    }

    dialog.vaultName = vault.name;
    dialog.mfwaUrl = vault.url;
    dialog.authentication = vault.authentication;
    dialog.initialize();
}
