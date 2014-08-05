
/**
 * Module dependencies
 */

let AssertionError = require('node/assert').AssertionError
  , callsite = require('util/callsite')
  , fs = require('fs')
  , env = require('env')

/**
 * Module exports
 */

export assert

/**
 * Assert `expr' is `true'
 *
 * @api public
 * @param {MIxed} expr
 */

module.exports = assert;
function assert (expr) {
  if (env.NO_ASSERT || Boolean(expr)) { return; }

  var stack = callsite();
  var call = stack[1];
  var file = call.getFileName();
  var lineno = call.getLineNumber();
  var src = fs.read(file);
  var line = src.split('\n')[lineno - 1];
  var match = line.match(/assert\((.*)\)/);
  var src = match ? match[0] : null;

  var err = new AssertionError({
    message: src,
    stackStartFunction: stack[0].fun
  });

  throw err;
}

