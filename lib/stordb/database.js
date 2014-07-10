
/**
 * `database' module
 */

export module database {

  /**
   * `Database' constructor
   *
   * @api public
   * @param {String} path
   */

  export function Database (path) {
    if (!(this instanceof Database)) {
      return new Database(path);
    }

    this.path = path;
    this.dbh = __ldb_new(this.path);
  }

  /**
   * Get a value by `key'
   *
   * @api public
   * @param {String} key
   */

  Database.prototype.get = function (key) {
    var value = __ldb_get(this.dbh, key);
    return value || null;
  };

  /**
   * Put `value' by `key'
   *
   * @api public
   * @param {String} key
   * @param {Mixed} value
   */

  Database.prototype.put = function (key, value) {
    return __ldb_put(this.dbh, key, value);
  };

  /**
   * Delete value by `key'
   *
   * @api public
   * @param {String} key
   */

  Database.prototype.del = function (key) {
    return __ldb_del(this.dbh, key);
  };

}

