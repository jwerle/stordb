
/**
 * Module dependencies
 */

let extend = require('util/extend')
let defineProperty = Object.defineProperty

/**
 * Property descriptor defauluts
 *
 * @api public
 */

export var defaults = {
  enumerable: false
};

/**
 * Lightweight property descriptor function
 *
 * @api public
 * @param {Object} object
 * @param {Mixed} value
 * @param {Object} descriptor - optional
 */

module.exports = define;
function define (object, value, descriptor) {
  defineProperty(object, value, extend(descriptor || {}, defaults));
}
