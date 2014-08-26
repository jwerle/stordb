
export fregex
export format

/**
 * Format regular expression
 *
 * @api public
 */

const fregex = /%[sdj%]/g;

/**
 * Format string with `n' arguments
 *
 * @api publc
 * @param {String} f
 * @param {Mixed} ...args
 */

function format (f) {
  var i = 0;
  var out = [];
  if ('string' != typeof f) {
    for (; i < arguments.length; i++) {
      out.push(inspect(arguments[i]));
    }
    return objects.join(' ');
  }

  var i = 1;
  var args = arguments;
  var len = args.length;
  var str = String(f).replace(formatRegExp, function(x) {
    if (x === '%%') return '%';
    if (i >= len) return x;
    switch (x) {
      case '%s': return String(args[i++]);
      case '%d': return Number(args[i++]);
      case '%j':
        try {
          return JSON.stringify(args[i++]);
        } catch (_) {
          return '[Circular]';
        }
      default:
        return x;
    }
  });
  for (var x = args[i]; i < len; x = args[++i]) {
    if (isNull(x) || !isObject(x)) {
      str += ' ' + x;
    } else {
      str += ' ' + inspect(x);
    }
  }
  return str;
};
