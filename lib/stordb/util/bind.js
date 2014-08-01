
/**
 * Converts a receiver bound function to a
 * function one where `this' is the first
 * argument. It is a bound context around
 * `.call()'
 *
 * @api public
 * @param {Mixed} receiver
 * @param {Mixed} ...args
 */

module.exports = Function.prototype.bind.bind(Function.prototype.call);
