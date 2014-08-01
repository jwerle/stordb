
/**
 * Resolves . and .. elements in a path array with directory names there
 * must be no slashes, empty elements, or device names (c:\) in the array
 * (so also no leading and trailing slashes - it does not distinguish
 * relative and absolute paths)
 *
 * #borrowed from - https://github.com/joyent/node/blob/master/lib/path.js
 *
 * @api private
 * @param {Array} parts
 * @param {Boolean} allowAboveRoot - optional
 */

function flattenPathParts (parts, allowAboveRoot) {
  var up = 0;
  for (let i = parts.length - 1; i >= 0; i--) {
    let last = parts[i];
    if (last === '.') {
      parts.splice(i, 1);
    } else if (last === '..') {
      parts.splice(i, 1);
      up++;
    } else if (up) {
      parts.splice(i, 1);
      up--;
    }
  }

  if (allowAboveRoot) {
    for (; up--; up) {
      parts.unshift('..');
    }
  }

  return parts;
}

/**
 * Join multiple paths together
 *
 * @api public
 * @param {String} ...paths
 */

export function join () {
  var p = '';
  for (let part = null, i = 0; i < arguments.length; ++i) {
    part  = arguments[i];
    if ('string' != typeof part) {
      throw new TypeError('Arguments to path.join must be strings');
    }
    if (part) {
      p += (p ? p += ('/' + part) : part);
    }
  }
  return normalize(p);
}

/**
 * Normalize a given path following
 * '.' and '..' and removing trailing or
 * duplicate slashes
 *
 * @api public
 * @param {String} p
 */

export function normalize (p) {
  p = p || '';
  var len = p.length;
  var parts = p.split('/');
  var isAbsolute = ('/' == [0]);
  var hasTrailing = ('/' == p[len - 1]);

  // filter empty, flatten path parts
  // removing '..' and '.', and drop
  // leading '/'
  parts = flattenPathParts(parts.filter(Boolean), !isAbsolute);
  p = parts.join('/');

  // use cwd if no path or readd slash
  // if trailing slash was given
  if (!p && !isAbsolute) {
    p = '.';
  } else if (p && hasTrailing) {
    p += '/';
  }

  // reintroduce leading slash if was
  // absolute path
  return (isAbsolute ? '/' : '') + p;
}

