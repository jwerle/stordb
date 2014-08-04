

/**
 * `Global' reference
 */

const global = this;

module boot {
  let nativeRequire = __native_require__
    , load = nativeRequire('__native_bindings__').sys_load
    , exit = nativeRequire('__native_bindings__').sys_exit

  let callsite = load('util/callsite').exports
  let program = null;
  let stack = null;

  /**
   * Initialize program
   *
   * @api public
   */

  export function init () {
    let argv = __ARGV__
    stack = callsite(init);
    initModules();
    cleanup();
    unwrap();
    program = global.program = require('program');
    exit(program.run(program.argv.parse(argv)));
  }

  function initModules () {
    load('bindings');
    load('env');
    load('require');
    load('console');
  }

  function cleanup () {
    delete global.__ARGV__;

    // remove native function calls
    for (let p in global) {
      if (0 == p.indexOf('__native_')) {
        delete global[p];
      }
    }
  }

  function unwrap () {
    for (let _ in boot) {
      global[_] = boot;
    }
  }

}

// init
boot.init();

