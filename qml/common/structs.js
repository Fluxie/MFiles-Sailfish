
.pragma library

/**
 * Assert the condition
 *
 * @param {bool} condition Condition to assert
 * @param {string} [msg] Message for the assertion error
 */
function _assert( condition, msg ) {
	if( !condition ) {
		msg = msg || 'Assertion failed';
		console.log( msg );
		console.trace();
		throw createError( msg || 'Assertion failed' );
	}
}

/**
 * Construct an error object
 *
 * @this {Error}
 * @class A common error object
 * @param {variant} msg
 *		Error message or an error struct containing the message.
 *		Struct should have 'message' or 'Message' field.
 */
function createError( msg ) {
	var error = new Error( msg.message || msg.Message || msg );
}

/**
 * Construct a Vault structure
 *
 * @this {Vault}
 * @class Stores vault connection information
 * @param {object} props Values for the vault members
 */
function Vault( props ) {
	_assert( props.guid && props.url && props.name && props.username );

	this.url = props.url;
	this.guid = props.guid;
	this.name = props.name;
	this.username = props.username;

	this.id = props.id;
	this.authentication = props.authentication || null;
}

