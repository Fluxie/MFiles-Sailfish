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

/**
 * Gets a HTTP client for the current vault
 *
 * @param {Page} page The main page for the vault
 * @returns {HttpClient} HttpClient configured for the vault
 */
function _getClient( page ) {
	var httpClient = new Http.HttpClient();
	httpClient.headers[ "X-Authentication" ] = page.authentication;
	httpClient.baseUrl = page.mfwaUrl + "/REST";
	return httpClient;
}

/**
 * Initialize the page
 *
 * @param {Page} page The main page
 * @param {SilicaListView} favorites Favorites list
 */
function initialize( page, favorites ) {

	// Initialize the vault cache.
	page.vault.initialize( page.mfwaUrl, page.authentication );

	// Get the favorites listing from M-Files
	var client = _getClient( page );
	client.get( '/favorites', function( err, favoriteObjects ) {

		// Append the individual results to the list model
		for( var f in favoriteObjects )
		{
			var ov = favoriteObjects[f];
			var mfclass = vault.get( 0, ov.Class );
			var cn = 'Loading...';
			if( mfclass )
				cn = mfclass.Name;
			var mfot = vault.get( 1, ov.ObjVer.Type );
			var otn = '';
			if( mfot )
				otn = mfot.Name;

			favorites.favoritesList.append({
				data: ov,
				title: ov.Title,
				className: cn,
				objectTypeName: otn

			});

		}  // end for

	});  // end get

}
