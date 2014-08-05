
/**
 * Module dependencies
 */

let extension = require('require').extension
  , path = require('node/path')
  , system = require('sys').system
  , sprintf = require('util/sprintf')
  , env = require('env')
  , ld = require('bindings')('ld')
  , fs = require('fs')

extension('.cc', function (_, mod) {
  let exports = {};
  let module = {exports: exports};
  let cc = 'g++';
  let src = mod;
  let objs = (mod.replace(/(\.cc|\.c)/, '')) + '.o';
  let out = (mod.replace(/(\.cc|\.c)/, '')) + '.stordb';

  if (!env.FORCE_EXT_COMPILE && fs.exists(path.resolve(out))) {
    open();
    return module;
  }

  function compile () {
    let cflags = [
      // include path
      '-Iinclude', '-Ileveldb/include', '-Iv8/include',
      // stordb path
      sprintf('-DSTORDB_JS_PATH="%s"', env.STORDB_PATH)
    ].join(' ');

    let cmd = sprintf(
      '%s %s %s -c -o %s',
      cc, src, cflags, objs
    );

    system(cmd);
  }

  function link () {
    let out = (mod.replace(/(\.cc|\.c)/, '')) + '.stordb';
    let cflags = [
      '-shared',
      '-std=gnu++11', '-stdlib=libstdc++',
      // include paths
      '-Iv8/include', '-Ileveldb/include',
      // libary paths
      '-L.',
      // linked libaries
      '-licudata',
      '-licui18n',
      '-licuuc',
      '-lstordb',
      '-lv8_base',
      //'-lv8_libbase',
      '-lv8_snapshot',
      '-lleveldb',
      sprintf('-DSTORDB_JS_PATH="%s"', env.STORDB_PATH)
    ].join(' ');

    let cmd = sprintf(
      '%s %s %s -o %s',
      cc, cflags, src, out
    );

    system(cmd);
  }

  function open () {
    module.exports = ld.open('./tests/ext.stordb');
  }

  compile();
  link();
  open();

  return module;
});



