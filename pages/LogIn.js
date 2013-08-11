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
 * Use the login details to select the vault.
 *
 * Displays the vault selection dialog to the user
 *
 * @param {string} url M-Files URL
 * @param {string} username Username
 * @param {string} password Password
 * @param {function} done Receives the error or selected vault information
 */
function selectVault( url, username, password, done ) {

	// Do login with the credentials
	performLogIn( url, username, password, function( err, vaults ) {
		if( err ) { return done( err ); }

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
				username: username,
				url: url,
				authentication: page.vaultAuthentication
			});
		});

		// Pop the vault select dialog away on reject.
		page.rejected.connect( function() {
			pageStack.pop();
			done();
		});
	});
}

/**
 * Performs login and retrieves the list of vaults
 *
 * @param {string} url M-Files URL
 * @param {string} username Username
 * @param {string} password Password
 * @param {function} done Receives the list of vaults
 */
function performLogIn( url, username, password, done ) {
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
function saveVault( vault, done ) {
	// DBAO.savePreviousVault( vault, done );
}

/**
 * Chooses a vault and moves to the vault home
 *
 * @param {object} vault Vault information
 */
function useVault( vault ) {

	// Make sure the authentication details are still valid.
	var client = new Http.HttpClient();
	client.headers[ 'X-Authentication' ] = vault.authentication;
	client.get( vault.url + '/REST/session', function( err, sessionInfo ) {
		var dialog;

		// If the auth token was invalid, we need to ask for new credentials.
		if( err ) {
			dialog = pageStack.push( "VaultPassword.qml" );
			dialog.username = vault.username;
			return;
		}

		// Auth token was valid, proceed to display the vault home.
		dialog = pageStack.replace( "VaultHome.qml" );
		dialog.vaultName = vault.name;
		dialog.mfwaUrl = vault.url;
		dialog.authentication = vault.authentication;
		dialog.initialize();
	});
}
