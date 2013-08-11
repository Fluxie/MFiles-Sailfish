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

.import 'utils.js' as Utils
.import 'Promise.js' as Promise

/**
 * RESTful HTTP client
 *
 * @constructor
 * @this {HttpClient}
 */
function HttpClient( options ) {

	this.baseUrl = "";

    // Headers are not stored as normal default options.
    this.headers = options && options.headers || {};
	if( options ) { delete options.headers; }

    this.defaultOptions = {
        contentType: 'application/json',
        serialize: 'json',
    }

    for( var o in options ) {
        this.defaultOptions[ o ] = options[ o ];
    }
};

/**
 * Sets a HTTP header for the future requests
 *
 * @param {string} name Header name
 * @param {string} value Header value
 */
HttpClient.prototype.setHeader = function( name, value ) {
    this.headers[ name ] = value;
}

/**
 * Perform a GET request
 *
 * @param {string} url Request URL
 * @param {object} [options] Request options
 * @param {function} [done] Node-style callback
 * @returns {Promise} Promise for queueing return handlers
 */
HttpClient.prototype.get = function( url, options, done ) {
    return this._performRequest( 'GET', url, null, options, done );
}

/**
 * Peform a DELETE request
 *
 * @param {string} url Request URL
 * @param {object} [options] Request options
 * @param {function} [done] Node-style callback
 * @returns {Promise} Promise for queueing return handlers
 */
HttpClient.prototype.delete = function( url, options, done ) {
    return this._performRequest( 'DELETE', url, null, options, done );
}

/**
 * Peform a POST request
 *
 * @param {string} url Request URL
 * @param {object} data POST data to be sent
 * @param {object} [options] Request options
 * @param {function} [done] Node-style callback
 * @returns {Promise} Promise for queueing return handlers
 */
HttpClient.prototype.post = function( url, data, options, done ) {
    return this._performRequest( 'POST', url, data, options, done );
}

/**
 * Peform a PUT request
 *
 * @param {string} url Request URL
 * @param {object} data PUT data to be sent
 * @param {object} [options] Request options
 * @param {function} [done] Node-style callback
 * @returns {Promise} Promise for queueing return handlers
 */
HttpClient.prototype.put = function( url, data, options, done ) {
    return this._performRequest( 'PUT', url, data, options, done );
}

/**
 * Performs the HTTP request
 *
 * @param {string} verb Request verb
 * @param {string} url Request URL
 * @param {object} data Data to be sent for POST and PUT requests
 * @param {object} options Request options
 * @param {function} done Node-style callback
 * @returns {Promise} Promise for the finished request
 */
HttpClient.prototype._performRequest = function( verb, url, data, options, done ) {
    // Handle (verb, url, data, done) args
    if( options instanceof Function ) { done = options; options = {}; }

    // Merge options with default ones
    options = Utils.merge( options || {}, this.defaultOptions );

    // Construct the XHR
    var promise = new Promise.Promise();
    var xhr = getXHR( promise );

    // Open and prepare the request
    xhr.open( verb, this.baseUrl + url );
    this._setHeaders( xhr, options.headers );

    // Handle data formats
    if( data ) {
        if( options.serialize === 'json' ) {
            data = JSON.stringify( data );
        }

        xhr.setRequestHeader( 'Content-Type', options.contentType );
    }

    // Send the request
    xhr.send( data );

    // If we got done callback, register it for the promise.
    // We need to mangle the parameters from the promise-style callback to
    // node-style callback here.
    if( done ) {
		console.log( "Registering Node callbacks" );
        promise.done( function( value, xhr ) { console.log( "Node OK" ); done( null, value, xhr ); } );
        promise.fail( function( err, xhr ) { console.log( "Node fail" );done( err, null, xhr ); } );
    }

    return promise;
};

/**
 * Construct the XHR and register the promise for it
 *
 * @param {Promise} promise Promise for when the request completes
 * @returns {XMLHttpRequest} XMLHttpRequest
 */
function getXHR( promise ) {

    // Create the XHR and register ready state change handler
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {

        // The request is completely finished on readyState 4
        if( xhr.readyState === 4 ) {

            // Unserialize the response content
            var responseType = xhr.getResponseHeader('Content-Type');
            var response = null;
            if( responseType.indexOf('/json') !== -1 ) {
                response = JSON.parse( xhr.responseText );
            }

            // Signal success on status 200, otherwise raise error.
            // Technically status <400 is success, but we don't support
            // redirects, etc. for now.
			console.log( "HTTP response, status: " + xhr.status );
			console.log( JSON.stringify( response, undefined, 2 ) );
            if( xhr.status === 200 ) {
                promise.signalDone( response, xhr );
            } else {
                console.log( "Error: " );
                console.log( JSON.stringify( response, undefined, 2 ) );
                promise.signalFail( response, xhr );
            }
        }
    };

    return xhr;
};

/**
 * Sets the default headers for the XHR
 *
 * @param {XMLHttpRequest} xhr
 * @param {object} additionalHeaders Additional headers to be set
 */
HttpClient.prototype._setHeaders = function( xhr, additionalHeaders ) {

    var headerName, headerValue;

    // Set the default headers
    for( headerName in this.headers ) {
        var headerValue = this.headers[ headerName ];
        xhr.setRequestHeader( headerName, headerValue );
    }

    // If no additional headers were specified, we're done here.
    if( !additionalHeaders ) { return; }

    // Set additional headers
    for( headerName in additionalHeaders ) {
        var headerValue = additionalHeaders[ headerName ];
        xhr.setRequestHeader( headerName, headerValue );
    }
};
