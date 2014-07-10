
module vm {

  /**
   * Run `fn' in `ctx'
   *
   * @api public
   * @param {Object} ctx
   * @param {Function} fn
   */

  export function run (ctx, fn) {
    return Function(
      'ctx', 'with (ctx) {'+
        'return ('+ fn.toString() +')();'+
      '}'
    )(ctx);
  }

}

