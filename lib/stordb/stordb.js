
import Database from database;

/**
 * `stordb' module
 */

export module stordb {
  print("in stordb module");

  export let Database = database.Database;

  var db = new Database('./test-db');

  db.put('foo', 'bar!!');
  print(db.get('foo'));
  db.del('foo');
  db.put('foo', 'kinkajou');
  print(db.get('foo'));
}
