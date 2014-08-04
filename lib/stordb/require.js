
let nativeRequire = __native_require__
  , nativePrint = __native_print__
  , nativeExit = __native_exit__
  , callsite = nativeRequire('util/callsite').exports

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
    cache[module] = nativeRequire(module);
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

