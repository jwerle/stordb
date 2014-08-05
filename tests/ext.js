
let ext = require('./tests/ext.cc')

export function main (argv) {
  ext.hello();
  return 0;
};
