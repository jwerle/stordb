
/**
 * Module dependencies
 */

let bindings = require('bindings')('io')
  , sprintf = require('util/sprintf')
  , bind = require('util/bind')

let shift = bind(Array.prototype.shift)
  , unshift = bind(Array.prototype.unshift)
  , slice = bind(Array.prototype.slice)

/**
 * File descriptors
 *
 * @api public
 */

export const stdin = bindings.stdin;
export const stdout = bindings.stdout;
export const stderr = bindings.stderr;

/**
 * Read `n' bytes from `fd'
 *
 * @api public
 * @param {Number} fd
 * @param {Number} n
 */

export function read (fd, n) {
  return bindings.read(fd, n || 1024);
}

/**
 * Write `n' bytes to `fd'
 *
 * @api public
 * @param {Number} fd
 * @param {Buffer|String} buf
 */

export function write (fd, buf) {
  return bindings.write(fd, buf, buf.length);
}

/**
 * Print formatted text to `fd'
 *
 * @api public
 * @param {Number} fd
 * @param {String} format
 * @param {Mixed} ...args
 */

export function fprintf () {
  var fd = null;
  var format = null;
  if (0 == arguments.length) {
    throw new TypeError("expecting file descriptor");
  } else if (1 == arguments.length) {
    fd = shift(arguments);
    format = "";
  } else if (2 == arguments.length) {
    fd = shift(arguments);
    format = shift(arguments);
  } else if (arguments.length > 2) {
    fd = shift(arguments);
    format = sprintf.apply(sprintf, arguments);
  }

  write(fd, format);
}

/**
 * Print formatted text to `stdout'
 *
 * @api public
 * @param {String} format
 * @param {Mixed} ...args
 */

export function printf () {
  unshift(arguments, stdout);
  fprintf.apply(null, arguments);
}
