
let ENV =  global.ENV;

if ('function' == typeof require) {
  ENV.STORDB_PATH = require('bindings')('STORDB').LIB_PATH;
}

for (let key in ENV) {
  exports[key] = String(ENV[key]);
}

/**
 * Get value form environment
 *
 * @api public
 * @param {String} key
 */

export function get (key) {
  return ENV[key] || null;
}

/**
 * Set `value' in environment by `key'
 *
 * @api public
 * @param {String} key
 * @param {String} value
 */

export function set (key, value) {
  ENV[key] = value;
  return this;
}

