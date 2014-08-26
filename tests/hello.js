
let env = require('env')

export function main (argv) {
  console.log(module)
  console.log([
    'user: %s',
    'argv: %s',
    'cwd: %s'
  ].join('\n'), env.USER, argv.join(', '), program.cwd());
  return 0;
}
