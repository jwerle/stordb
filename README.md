stordb
======

StorDB = LevelDB + V8 (ES6)

## status

WIP

## building

```sh
$ make dependencies
$ PREFIX=/usr/local make
$ PREFIX=/usr/local make install
```

## example

```js

let ldb = require('ldb')

export function main (argv) {
  let db = ldb.open('/path/to/leveldb');
  db.put('foo', 'bar');
  console.log(db.get('foo'));
}
```

## modules

`stordb` makes use of ES6 module features. The `export` function is
currently supported and all stordb modules are compat with CommonJS. The
`export` keyword attaches its operand to the exports object.

*person.js*

```js

export Person

function Person (name) {
  this.name = name;
}
```
*main.js*

```js
let Person = require('./person').Person

export main (argv) {
  let joe = Person(argv[3]);
  console.log(joe.name);
}
```

Modules can be defined in modules with the `module` keyword:

*tcp.js*

```js
module client {
  Client (addr) {
    // magic
  }

  export function create (addr) {
    return new Client(addr);
  }
}

module server {
  function Server () {
    // magic
  }

  Server.prototype.listen = function (port) {
    // listen
  };

  export function create () {
    return new Server();
  }
}
```

*main.js*

```js
let tcp = require('./tcp')

let client = tcp.client.create('0.0.0.0:5432');
let server = tcp.server.create();

server.listen(5432);
```

## license

MIT
