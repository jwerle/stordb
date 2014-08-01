
/**
 * Module dependencies
 */

let minimist = require('ext/minimist')
  , callsite  = require('ext/callsite')
  , path = require('node/path')
  , io = require('io')
  , exit = require('sys').exit
  , chdir = require('sys').chdir
  , cwd = require('sys').cwd

export cwd
export chdir
export exit

/**
 * Program version
 *
 * @api public
 *
 */
export const VERSION = require('bindings')('STORDB').VERSION;

/**
 * `run' function
 *
 * @api public
 * @param {Array} argv
 */

export function run (argv) {
  var args = minimist(argv);
  {
    // flags
    for (let flag in args) {
      switch (flag) {
        case 'h':
          usage.output();
          exit(0);
          break;

        case 'V':
          console.log(VERSION);
          exit(0);
          break;

        case 'e':
          {
            let src = args[flag];
            let result = script.compile(src);
            if (null != result.error) {
              console.error("error: %s", result.error);
              if (result.error.stack) {
                console.error(result.error.stack);
              }
            }
          }
          break

        case '_': break;
        default:
          console.error("error: Unknown option `%s'", flag);
          usage.output();
          exit(1);
      }
    }


    // scripts
    var main = null;
    for (var i = 1; i < args._.length; ++i) {
      if (!Boolean(args._[i])) { continue; }
      var file = args._[i];
      var mod = require(file);
      if ('function' == typeof mod.main) {
        if (null == main) {
          main = mod.main;
        } else {
          throw new TypeError("`main(argv) { ... }' already defined");
        }
      }
    }

    if ('function' == typeof main) {
      return main(program.argv.value()) | 0;
    }
  }
  return 0;
}


/**
 * Usage commands
 *
 * @api public
 */

export module usage {
  let p = console.error;
  export function output () {
    p("usage: stordb [-hV] [...scripts] [-e code]");
  }
}


/**
 * program commands
 *
 * @api public
 */

export module commands {
  export function help () {

  }
}

/**
 * `argv' sub module for `program'
 *
 * @api public
 */

export module argv {

  /**
   * `argv' parsed state
   *
   * @api private
   */

  let value_ = null;

  /**
   * Parse and set value from
   * `argv' string
   *
   * @api public
   * @param {String} str
   */

  export function parse (str) {
    if ('string' != typeof str) {
      throw new TypeError("expecting string");
    }
    return (value_ = str.split('!!'));
  }

  /**
   * Get value at index or argv array
   *
   * @api public
   * @param {Number} i - optional
   */

  export function get (i) {
    null == i ? value_ : value_[i];
  }

  export function toString () {
    return value_.join(' ');
  }

  export function valueOf () {
    return value_;
  }

  export function value () {
    return valueOf();
  }
}

/**
 * script module
 *
 * @api public
 */

export module script {

  /**
   * Load and run script
   *
   * @api public
   * @param {String} path
   */

  export function load (path) {
    try { require(path) }
    catch (e) {
      console.error(e)
    }
  }

  /**
   * Compile and eval source code in new
   * context
   *
   * @api public
   * @param {String} src
   */

  export function compile (src) {
    var stack = callsite();
    try {
      return {
        result: Function(src).call(global),
        stack: stack
      };
    } catch (e) {
      return {
        error: e,
        stack: stack
      };
    }
  }
}

