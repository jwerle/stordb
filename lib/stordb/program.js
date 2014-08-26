
/**
 * Module dependencies
 */

let argv = require('argv')
  , sys = require('sys')

export var exit = sys.exit
export var chdir = sys.chdir
export var cwd = sys.cwd

/**
 * Program version
 *
 * @api public
 *
 */

export const VERSION = require('bindings')('STORDB').VERSION;
export var version = VERSION;

/**
 * `run' function
 *
 * @api public
 */

export function run () {
  {
    let main = null;

    if ('function' == typeof main) {
      return main(argv) | 0;
    } else {
      // flags
      for (let flag in argv) {
        switch (flag) {
          case '_': break;

          case 'h':
            usage.output();
            exit(0);
            break;

          case 'V':
            console.log(VERSION);
            exit(0);
            break;

          case 'e': {
            let src = argv[flag];
            let result = script.compile(src);
            if (null != result.error) {
              console.error("error: %s", result.error);
              if (result.error.stack) {
                console.error(result.error.stack);
              }
            }
            return 0;
          }
          break;

          default:
            console.error("error: Unknown option `%s'", flag);
            usage.output();
            exit(1);
        }
      }
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
    try { require('require').exports(path) }
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
    try {
      return {
        result: Function(src).call(global)
      }
    } catch (e) {
      return {
        error: e,
        stack: e.stack
      };
    }
  }
}

