
/**
 * Module dependencies
 */

var path = require('node/path')
  , bindings = require('bindings')('fs')

/**
 * Module exports
 */

export File
export Stats
export open
export error
export rename
export stat
export lstat
export truncate
export chown
export size
export read
export write
export mkdir
export rmdir
export exists

/**
 * `File' constructor
 *
 * @api public
 * @param {String} path
 */

function File (path) {
  if (!(this instanceof File)) {
    return new File(path);
  }

  if ('string' != typeof path) {
    throw new TypeError("expecting `path' to be a string");
  }

  this.path = path;
  this.opened = false;
  this.ref = null;
}

/**
 * Opens file at `path' with `flags'
 * set
 *
 * @api public
 * @param {String} flags
 */

File.prototype.open = function (flags) {
  if ('string' != typeof flags) {
    throw new TypeError("expecting string");
  } else if (this.opened) {
    //throw new Error("File already opened");
  }

  this.ref = bindings.open(this.path, flags);
  this.opened = Boolean(this.ref);
  return this;
};

/**
 * Rename file path
 *
 * @api public
 * @param {String} to
 */

File.prototype.rename = function (to) {
  //if (this.opened) { throw new Error(""); }
  bindings.rename(this.path, to);
  this.path = to;
  return this;
};

/**
 * Return stats for file
 *
 * @api public
 */

File.prototype.stat = function () {
  if (this.opened) {
    return Stats(this.ref);
  } else {
    return Stats(this.path);
  }
};

/**
 * Truncates file with `len' offset
 *
 * @api public
 * @param {Number} len
 */

File.prototype.truncate = function (len) {
  //if (this.opened) {
    //bindings.ftruncate(this.ref, len);
  //} else {
    //bindings.truncate(this.path, len);
  //}
  bindings.truncate(this.path, len);
  return this;
};

/**
 * Change ownership of file
 *
 * @api public
 * @param {Number} uid
 * @param {Number} gid
 */

File.prototype.chown = function (uid, gid) {
  if (this.opened) {
    return bindings.fchown(this.ref, uid, gid);
  } else {
    return bindings.chown(this.path, guid, gid);
  }
};

/**
 * Get size of file
 *
 * @api public
 */

File.prototype.size = function () {
  if (this.opened) {
    return bindings.fsize(this.ref);
  } else {
    return bindings.size(this.path);
  }
};

/**
 * Read contents of file
 *
 * @api public
 * @param {Number} len - optional
 */

File.prototype.read = function (len) {
  if (this.opened) {
    return len ?
      bindings.fnread(this.ref, len) :
      bindings.fread(this.ref);

  } else {
    return len ?
      bindings.nread(this.path, len || this.size()) :
      bindings.read(this.path);
  }
};

/**
 * Write contents to file
 *
 * @api publc
 * @param {Mixed} buf
 * @param {Number} len - optional
 */

File.prototype.write = function (buf, len) {
  if (this.opened) {
    return null != len ?
      bindings.fnwrite(this.ref, buf, len) :
      bindings.fwrite(this.ref, buf);
  } else {
    return null != len ?
      bindings.nwrite(this.path, buf, len) :
      bindings.write(this.ref, buf);
  }
};

/**
 * Checks if file exists
 *
 * @api public
 */

File.prototype.exists = function () {
  return bindings.exists(this.path);
};

/**
 * Close opened file
 *
 * @api public
 */

File.prototype.close = function () {
  if (this.opened) {
    this.opened = false;
    return bindings.close(this.ref);
  }
  return false;
}

/**
 * Seek to a position in an opened
 * file
 *
 * @api publc
 * @param {Number} offset - optional (Default: 0)
 * @param {Number} whence - optional
 */

File.prototype.seek = function (offset, whence) {
  if (this.opened) {
    bindings.seek(this.ref, offset|0, whence|0);
  }
  return this;
};

/**
 * `Stat' constructor
 *
 * @api public
 * @param {String|Object} path
 */

function Stats (path) {
  var stats = null;
  if (!(this instanceof Stats)) {
    return new Stats(path);
  } else if ('object' == typeof path) {
    stats = bindings.fstat(path);
  } else if ('string' == typeof path) {
    stats = bindings.stat(path);
  } else {
    throw new TypeError("expecting string path or object reference (FILE *)");
  }

  if (null == stats) {
    error("Stats()");
    return null;
  }

  this.dev = stats.st_dev;
  this.ino = stats.st_ino;
  this.mode = stats.st_mode;
  this.nlink = stats.st_nlink;
  this.uid = stats.st_uid;
  this.gid = stats.st_gid;
  this.rdev = stats.st_rdev;
  this.size = stats.st_size;
  this.blocks = stats.st_blocks;
  this.blksize = stats.st_blksize;
  this.flags = stats.st_flags;
  this.gen = stats.st_gen;

  this.atime = stats.st_atime
  this.mtime = stats.st_mtime
  this.ctime = stats.st_ctime
}

/**
 * Prints last `fs' error to `stderr'
 *
 * @api public
 * @param {String} prefix - optional
 */

function error (prefix) {
  bindings.error(prefix);
}

/**
 * Open file at `path' with
 * `flags'
 *
 * @api public
 * @param {String} path
 * @param {String} flags
 */

function open (path, flags) {
  return File(path).open(flags);
}

/**
 * Move a path to a new destination
 *
 * @api public
 * @param {String} from
 * @param {String} to
 */

function rename (from, to) {
  return File(from).rename(to);
}

/**
 * Returns instance of `Stats'
 * for a given file
 *
 * @api public
 * @param {String} path
 */

function stat (path) {
  return File(path).stat();
}

/**
 * Stats a given link at `path'
 *
 * @api public
 * @param {String} path
 */

function lstat (path) {
  return bindings.lstat(path);
}

/**
 * Truncate file at path with `len' offset
 *
 * @api public
 * @param {String} path
 * @param {Number} len
 */

function truncate (path, len) {
  return File(path).truncate(len);
}

/**
 * Change ownership of file at `path'
 *
 * @api public
 * @param {String} path
 * @param {Number} uid
 * @param {Number} gid
 */

function chown (path, uid, gid) {
  return File(path).chown(uid, gid);
}

/**
 * Return eise of file at `path'
 *
 * @api public
 * @param {String} path
 */

function size (path) {
  return File(path).size();
}

/**
 * Reads contents of file at `path'
 * with optional length
 *
 * @api public
 * @param {String} path
 * @param {Number} len - optional
 */

function read (path, len) {
  return File(path).read(len);
}

/**
 * Write buffer to file at `path'
 *
 * @api public
 * @param {String} path
 * @param {Mixed} buffer
 */

function write (path, buffer) {
  return File(path).write(buffer);
}

/**
 * Make directory at `path'
 *
 * @api public
 * @param {String} path
 */

function mkdir (path) {
  return bindings.mkdir(path);
}

/**
 * Remove directory at `path'
 *
 * @api public
 * @param {String} path
 */

function rmdir (path) {
  return bindings.rmdir(path);
}

/**
 * Check if file exists at `path'
 *
 * @api public
 * @param {String} path
 */

function exists (path) {
  return File(path).exists();
}

