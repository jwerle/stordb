
var env = require('env')

export function main (argv) {
  console.log('hello');
  console.log('%s', env.get('USER'));
  return 0;
}
