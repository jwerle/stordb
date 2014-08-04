
let fs = require('fs')
  , ok = require('util/ok')
  , assert = require('node/assert')
  , resolve = require('node/path').resolve

export function main (argv) {
  // `Stats'
  {
    let stats = fs.Stats(resolve(__DIR__, './fixtures/file'));
    assert(!(null == stats.dev));
    assert(!(null == stats.ino));
    assert(!(null == stats.mode));
    assert(!(null == stats.nlink));
    assert(!(null == stats.uid));
    assert(!(null == stats.gid));
    assert(!(null == stats.rdev));
    assert(!(null == stats.size));
    assert(!(null == stats.blocks));
    assert(!(null == stats.blksize));
    assert(!(null == stats.flags));
    assert(!(null == stats.gen));
    assert(!(null == stats.atime));
    assert(!(null == stats.mtime));
    assert(!(null == stats.ctime));
    ok('Stats');
  }

  // `File'
  {
    let path = resolve(__DIR__, './fixtures/file');
    let file = fs.File(path);
    assert(file);

    // `File#open'
    {
      file.open('a+');
      assert(file.opened);

      ok('File#open');
    }

    // `File#rename'
    {
      let to = resolve(__DIR__, './fixtures/foo');
      let from = file.path;
      file.rename(to);
      assert(to == file.path);
      file.rename(from);
      assert(from == file.path);
      ok('File#rename');
    }

    // `File#stat'
    {
      let stats = file.stat();
      assert(stats);
      ok('File#stat');
    }


    // `File#chown'
    {
      let prev = file.stat();
      let cur = null;
      file.chown(501, 20);
      cur = file.stat();
      file.chown(prev.uid, prev.gid);
      cur = file.stat();
      assert(cur.uid = prev.uid);
      assert(cur.gid = prev.gid);
      ok('File#chown');
    }

    // `File#size'
    {
      assert(0 == file.size());
      ok('File#size');
    }

    // `File#read'
    {
      assert('' == file.read());
      ok('File#read');
    }

    // `File#write'
    {
      assert(3 == file.write('foo'));
      file.seek(0);
      assert('foo' == file.read(3));
      file.truncate();
      assert('' == file.read());
      ok('File#write');
    }

    // `File#exists'
    {
      assert(file.exists());
      ok('File#exists');
    }

    // `File#truncate'
    {
      file.truncate();
      ok('File#truncate');
    }

    // `File#close'
    {
      assert(file.close());
      ok('File#close');
    }

    ok('File');
  }


  // `fs'
  {
    let file = fs.open('./stordb', 'rw');
    assert(file);
    ok('fs.open');
  }

  return ok.done(), 0;
}
