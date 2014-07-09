
//import foo from B;
import Database from database;

print(this.foo);
print(JSON.stringify(this))
/**
 * Module dependencies
 */


module B {
  export function foo () {}
}

/**
 * `stordb' module
 */

export module stordb {
  print("in stordb module");
  //Database();
  for (var i = 0; i < 1000000; ++i) {
    //print(i);
  }

  stordb.StorDB = StorDB;
  export function StorDB (path) {
    if (!(this instanceof StorDB)) {
      return new StorDB(path);
    }
  }

  /**
   * Get value by `key'
   *
   * @api public
   * @param {String|Number} key
   * @return {Mixed}
   */

  StorDB.prototype.get = function (key) {

  };

  /**
   * Set `value' by `key'
   *
   * @api public
   * @param {String} key
   * @param {Mixed} value
   * @return {Boolean}
   */

  StorDB.prototype.set =
  StorDB.prototype.put = function (key, value) {

  };

  /**
   * Remove value by `key'
   *
   * @api public
   * @param {String|Number} key
   * @return {Boolean}
   */

  StorDB.prototype.delete =
  StorDB.prototype.del = function (key) {

  };

}
