
/**
 * Module dependencies
 */

let format = require('_vendor/util').format
  , io = require('io')

let bind = Function.prototype.bind.bind(Function.prototype.call);
let each = bind(Array.prototype.forEach)
  , map = bind(Array.prototype.map)

/**
 * `Console' constructor
 *
 * @api publc
 * @param {String} name
 */

module.exports = new Console("Global");
export function Console (name) {
  if (!(this instanceof Console)) {
    return new Console(naem);
  }
  this.name = name;
}

/**
 * Log formatted output to `stdout'
 *
 * @api publc
 * @param {String} format
 * @param {String} ...args
 */

Console.prototype.log = function () {
  io.write(io.stdout, format.apply(null, arguments));
  io.write(io.stdout, '\n');
};

/**
 * Log formatted error to `stderr'
 *
 * @api public
 * @param {String} format
 * @param {String} ...args
 */

Console.prototype.error = function () {
  io.write(io.stderr, format.apply(null, arguments));
  io.write(io.stderr, '\n');
};

