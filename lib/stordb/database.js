
/**
 * Module dependencies
 */

var bindings = require('bindings')('db')

export open
export Database

/**
 * Open database
 */

module.exports = open;
function open (path) {
  return Database(path).open();
}

/**
 * `Database' constructor
 *
 * @api public
 * @param {String} path
 */

function Database (path) {
  if (!(this instanceof Database)) {
    return new Database(path);
  } else if ('string' != typeof path) {
    throw new TypeError("expecting string");
  }

  this.ref = null;
  this.path = path;
  this.opened = false;
}

Database.prototype.open = function () {
  this.ref = bindings.open(this.path);
  this.opened = Boolean(this.ref);
  return this;
};

