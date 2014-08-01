
/**
 * `Global' reference
 */

const global = this;


module boot {


/**
 * Runtime bindings
 */

  let sys = __sys_load__('bindings').exports('sys')
    , load = sys.load
    , exit = sys.exit
    , callsite = load('ext/callsite').exports

  let program = null;
  let stack = null;

  /**
   * Constants
   */

  export const STORDB_PATH = load('bindings').exports('STORDB').LIB_PATH;

  /**
   * Initialize program
   *
   * @api public
   */

  export function init () {
    stack = callsite(init);
    initModules();
    unwrap();
    program = global.program = require('program');
    exit(program.run(program.argv.parse(__ARGV__)));
  }

  /**
   * Initialize modules
   */

  function initModules () {
    load('require');
    load('console');
  }

  function unwrap () {
    for (let _ in boot) {
      global[_] = boot;
    }
  }

}

// init
boot.init();

