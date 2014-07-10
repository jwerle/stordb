
/**
 * `stordb' module
 */

export module stordb {
  //import Database from database;

  /**
   * Opens and returns a new `Database'
   * instance
   *
   * @api public
   * @param {String} path
   */

  export function open (path) {
    return database.Database(path);
  }

}

if (env.get('TEST')) {
  print(env.get('USER')); // jwerle
  var db = stordb.open('/tmp/test-db');
  db.put('foo', 'bar!!');
  print(db.get('foo'));
  db.del('foo');
  db.put('foo', 'kinkajou');
  print(db.get('foo'));
}
