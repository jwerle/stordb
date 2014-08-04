
let sys = require('sys')
  , env = require('env')

export function main (argv) {
  console.log('sys.print()');
  sys.print('foo');

  console.log('sys.load()');
  sys.print(sys.load(__DIR__ + '/noop'));

  console.log('sys.chdir()');
  sys.chdir(env.HOME);
  sys.print(sys.cwd());

  console.log('sys.cwd()');
  sys.print(sys.cwd());

  console.log('sys.access()');
  console.log(sys.access(env.HOME, sys.R_OK|sys.W_OK))

  console.log('sys.exit()');
  sys.exit(0);
  return 0;
}
