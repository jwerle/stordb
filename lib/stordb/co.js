
/**
 * Module dependencies
 */

let coro = require('bindings')('coro')

/**
 * Exports
 */

export co

/**
 * Run a function in another context
 *
 * @api public
 * @param {Function} fn
 */

module.exports = co;
function co (fn) {
  var ctx = coro.create(fn);
  return function () {
    coro.enter(ctx);
  };
}
