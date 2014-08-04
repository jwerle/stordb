
let system = require('sys').system
  , sprintf = require('util/sprintf')

export function main (argv) {
  return system(argv.slice(2).join(' '));
}
