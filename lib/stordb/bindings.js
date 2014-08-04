
/**
 * Native module bindings
 */

let modules = __native_require__('__native_bindings__');

/**
 * Binding map
 */

let map = null;

/**
 * Returns binding map for a given
 * namespace
 *
 * @api public
 * @param {String} ns
 */

module.exports = bindings
export function bindings (ns) {
  if (null == map) {
    map = Object.create(null);
    let prop = null;
    for (prop in modules) {
      let match = null;
      let ns = null;
      let value = null;
      let ref = null;

      match = prop.match(/^([a-zA-Z]+)_([a-zA-Z0-9|_]+)$/);

      if (null == match) { continue; }
      match = match.splice(1);
      ns = match.shift();
      value = match.shift();
      ref = modules[prop];
      if (null == map[ns]) {
        map[ns] = Object.create(null);
      }

      switch (typeof ref) {
        case 'function': ref = ref.bind(modules); break;
      }

      map[ns][value] = ref;
    }
  }
  return map[ns];
};

