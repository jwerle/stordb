
/**
 * Module dependencies
 */

let format = require('node/util').format
  , bind = require('util/bind')
  , io = require('io')
  , define = require('util/define')

let each = bind(Array.prototype.forEach)
  , map = bind(Array.prototype.map)

/**
 * `Console' constructor
 *
 * @api publc
 * @param {String} name
 */

global.console = module.exports = new Console("Global");
export function Console (name) {
  if (!(this instanceof Console)) {
    return new Console(naem);
  }
  define(this, 'name', {value: name});
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

