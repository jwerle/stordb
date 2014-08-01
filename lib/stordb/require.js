
/**
 * Module cache
 *
 * @api private
 */

let cache = {};

/**
 * Naive require function
 *
 * @api public
 * @param {String} module
 */

global.require = require;
export function require (module) {
  if (null == cache[module]) {
    cache[module] = __sys_load__(module);
  }
  return cache[module].exports;
}

/**
 * Purge module cache
 *
 * @api public
 */

export function purge () {
  cache = {};
}

