
let nativeRequire = global.__native_require__ || require
  , callsite = nativeRequire('util/callsite').exports
  , path = nativeRequire('node/path').exports
  , bind = nativeRequire('util/bind').exports

export require
export purge
export extension

var extname = path.extname;
var isArray = Array.isArray;
var each = bind(Array.prototype.forEach);

/**
 * Module cache
 *
 * @api private
 */

let cache = {};

/**
 * Extension handler map
 */

let extensions = {};

/**
 * Naive require function
 *
 * @api public
 * @param {String} module
 */

module.exports = require
function require (module) {
  let ext = null;
  if ('require' == module) {
    return require;
  }

  ext = extname(module);
  if (extensions[ext]) {
    cache[module] = extensions[ext](ext, path.resolve(module));
  } else if (null == cache[module]) {
    cache[module] = nativeRequire(module);
  }
  return cache[module].exports;
}

/**
 * Purge module cache
 *
 * @api public
 */

function purge () {
  cache = {};
}

/**
 * Extend or get extension support
 *
 * @api public
 * @param {String|Object|Array} ext
 * @param {Function} fn
 */

function extension (ext, fn) {
  if ('string' == typeof ext && null == fn) {
    return extensions[ext] || null;
  } else if (isArray(ext)) {
    each(ext, function (e) { extension(e, fn); });
  } else if ('object' == typeof ext) {
    for (let key in ext) { extension(key, ext[key]); }
  } else if ('string' == typeof ext && 'function' == typeof fn) {
    extensions[ext] = fn;
  }
}

