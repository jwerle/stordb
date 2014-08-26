
//var print = __native_print__;

/**
 * Startup boot module
 */

module boot {

  /**
   * Global object refrence
   */

  this.global = this;

  /**
   * Top level module
   */

  let module = null;

  /**
   * Boot require
   */

  let require = null;

  /**
   * Initializes runtime - called internally
   *
   * @api public
   */

  export function init () {
    startup();
    globals();
    program.run();
  }

  /**
   * Startup routines
   *
   * @api private
   */

  function startup () {
    module = new Module('boot.js');
    require = module.require.bind(module);
  }

  /**
   * Global bindings
   *
   * @api privte
   */

  function globals () {
    global.module = module;
    global.console = require('console');
    global.program = require('program');
  }

}

/**
 * Module cache
 */

Module.cache = {};

/**
 * Module constructor
 *
 * @api public
 * @param {String} filename
 */

function Module (filename) {
  this.filename = filename;
  this.dirname = filename.split('/').slice(0, -1).join('/');
  this.exports = {};
  this.children = [];
  this.parent = null;
}

/**
 * Requires a module at path relative to
 * module
 *
 * @api public
 * @param {String} path
 */

Module.prototype.require = function (path) {
  path = this.resolve(path);
  var mod = (
    Module.cache[path] = Module.cache[path] || __native_require__(path)
  );
  mod.parent = this;
  this.children.push(mod);
  return mod.exports;
};

/**
 * Resolve file path at module scope path
 *
 * @api public
 * @param {String} path
 */

Module.prototype.resolve = function (path) {
  var tmp = null;
  var ext = null;
  if ('.' == path[0]) {
    if ('/' == path[1]) {
      path = ('/'+
        this.dirname.split('/')
        .filter(Boolean)
        .concat(path.split('./').filter(Boolean))
        .join('/')
      );
    } else if ('.' == path[1]) {
      tmp = this.dirname.split('/').filter(Boolean);
      path.split('../').forEach(function (part) {
        if (!Boolean(part)) {
          tmp.pop();
        } else {
          tmp.push(part);
        }
      });
      path = ('/'+ tmp.filter(Boolean).join('/'));
    } else {
      throw new Error("Valid file id \`'"+ path);
    }
  }
  return path;
};

/**
 * Yield boot to runtime
 */

(boot.init());

