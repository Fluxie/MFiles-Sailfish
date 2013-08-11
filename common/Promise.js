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
 * Promise constructor
 *
 * @constructor
 * @this {Promise}
 */
function Promise() {

    // Set up the waiting queues
    this.queue = [];
    this.events = {};
    this.cbArgs = null;
};

/**
 * Register a done callback
 *
 * @param {function} cb Callback called if the promise is successful.
 * @returns {Promise} this
 */
Promise.prototype.done = function( cb ) {
    this._registerCallback( 'done', cb );
    return this;
};

/**
 * Register a fail callback
 *
 * @param {function} cb Callback called if the promise fails.
 * @returns {Promise} this
 */
Promise.prototype.fail = function( cb ) {
    this._registerCallback( 'fail', cb );
    return this;
};

/**
 * Register an always callback
 *
 * @param {function} cb
 *      Callback called when the promise is fulfilled no matter the result.
 * @returns {Promise} this
 */
Promise.prototype.always = function( cb ) {
    this._registerCallback( 'always', cb );
    return this;
};

/**
 * Registers the callback
 *
 * @param {string} status Callback status requirement
 * @param {function} cb Callback
 */
Promise.prototype._registerCallback = function( status, cb ) {

    // Check if the promise has happened.
    if( this.queue ) {

        // We still have a queue so the promise hasn't happened.
        // Add the callback to the queue.
        this.queue.push( { on: status, callback: cb } );
        return;

    }

    // Promise has happened. If the status matches one of the events,
    // call it immediately.

    if( this.events[ status ] ) {

        // If the status equals done
        cb.apply( null, this.cbArgs );
    }
}

/**
 * Signals that the promise is done.
 *
 * This should be called by the creator of the Promise.
 *
 * @param {...object} var_args Parameters passed to the callback
 */
Promise.prototype.signalDone = function() {
    this._signalHandlers( arguments, [ 'done', 'always' ] );
};

/**
 * Signals that the promise failed.
 *
 * This should be called by the creator of the Promise.
 *
 * @param {...object} var_args Parameters passed to the callback
 */
Promise.prototype.signalFail = function() {
    this._signalHandlers( arguments, [ 'fail', 'always' ] );
};

/**
 * Signals the callback handlers on the promise status.
 *
 * @param {object[]} args Parameters passed to the callback
 * @param {string[]} events Events that should be signaled
 */
Promise.prototype._signalHandlers = function( args, events ) {
    if( !this.queue ) { throw 'Promise cannot happen twice'; }

    // Store the callback args and set up the events so we can refer to
    // them later.
    this.cbArgs = args;
    this.events = {};
    for( var e in events ) { this.events[ events[e] ] = true; }

    // Call all the handlers in the queue.
    for( var i in this.queue ) {
        var handler = this.queue[i];
        if( this.events[ handler.on ] ) {
            handler.callback.apply( null, args );
        }
    }

    // Remove the queue so promise callbacks are invoked immediately.
    this.queue = null;
};

