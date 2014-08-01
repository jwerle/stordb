
/**
 * Binding map
 */

let map = Object.create(null);

{
  let prop = null;
  for (prop in global) {
    let match = null;
    let ns = null;
    let value = null;
    let ref = null;

    match = prop.match(/^__([a-zA-Z]+)_([a-zA-Z0-9|_]+)__$/);

    if (null == match) { continue; }
    match = match.splice(1);
    ns = match.shift();
    value = match.shift();
    ref = global[prop];
    if (null == map[ns]) {
      map[ns] = Object.create(null);
    }

    switch (typeof ref) {
      case 'function': ref = ref.bind(global); break;
    }

    map[ns][value] = ref;
  }
}

/**
 * Returns binding map for a given
 * namespace
 *
 * @api public
 * @param {String} ns
 */

module.exports = bindings
export function bindings (ns) {
  return map[ns];
};

