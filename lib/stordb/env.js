
/**
 * Get value form environment
 *
 * @api public
 * @param {String} key
 */

export function get (key) {
  return __ENV__[key] || null;
}

/**
 * Set `value' in environment by `key'
 *
 * @api public
 * @param {String} key
 * @param {String} value
 */

export function set (key, value) {
  __ENV__[key] = value;
  return this;
}

