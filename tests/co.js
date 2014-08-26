
let co = require('co')

export function main (argv) {
  co(function *() {
    console.log(argv);
  })();
}
