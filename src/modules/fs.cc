
#include <errno.h>
#include "stordb.h"
#include "stordb/macro.h"
#include "stordb/module.h"
#include "fs.h"

extern "C" {
#include <fs/fs.h>
}

static void
wrap_stats (v8::Handle<v8::ObjectTemplate> stats, fs_stats *fstats) {
  stats->Set(V8STRING("st_dev"), V8NUMBER((long long) fstats->st_dev));
  stats->Set(V8STRING("st_ino"), V8NUMBER((long long) fstats->st_ino));
  stats->Set(V8STRING("st_mode"), V8NUMBER((long long) fstats->st_mode));
  stats->Set(V8STRING("st_nlink"), V8NUMBER((long long) fstats->st_nlink));
  stats->Set(V8STRING("st_uid"), V8NUMBER((long long) fstats->st_uid));
  stats->Set(V8STRING("st_gid"), V8NUMBER((long long) fstats->st_gid));
  stats->Set(V8STRING("st_rdev"), V8NUMBER((long long) fstats->st_rdev));
  stats->Set(V8STRING("st_size"), V8NUMBER((long long) fstats->st_size));
  stats->Set(V8STRING("st_blocks"), V8NUMBER((long long) fstats->st_blocks));
  stats->Set(V8STRING("st_blksize"), V8NUMBER((long long) fstats->st_blksize));
  stats->Set(V8STRING("st_flags"), V8NUMBER((long long) fstats->st_flags));
  stats->Set(V8STRING("st_gen"), V8NUMBER((long long) fstats->st_gen));

  stats->Set(V8STRING("st_atime"), V8DATE((double) (fstats->st_atime * 1000)));
  stats->Set(V8STRING("st_mtime"), V8DATE((double) (fstats->st_mtime * 1000)));
  stats->Set(V8STRING("st_ctime"), V8DATE((double) (fstats->st_ctime * 1000)));
}

/**
 * Exports
 */

STORDB_MODULE(fs, {
  STORDB_MODULE_SET(fs, "error", V8FUNCTION(stordb_fs_berror));
  STORDB_MODULE_SET(fs, "open", V8FUNCTION(stordb_fs_bopen));
  STORDB_MODULE_SET(fs, "close", V8FUNCTION(stordb_fs_bclose));
  STORDB_MODULE_SET(fs, "rename", V8FUNCTION(stordb_fs_brename));
  STORDB_MODULE_SET(fs, "stat", V8FUNCTION(stordb_fs_bstat));
  STORDB_MODULE_SET(fs, "fstat", V8FUNCTION(stordb_fs_bfstat));
  STORDB_MODULE_SET(fs, "lstat", V8FUNCTION(stordb_fs_blstat));
  STORDB_MODULE_SET(fs, "truncate", V8FUNCTION(stordb_fs_btruncate));
  STORDB_MODULE_SET(fs, "ftruncate", V8FUNCTION(stordb_fs_bftruncate));
  STORDB_MODULE_SET(fs, "chown", V8FUNCTION(stordb_fs_bchown));
  STORDB_MODULE_SET(fs, "fchown", V8FUNCTION(stordb_fs_bfchown));
  STORDB_MODULE_SET(fs, "lchown", V8FUNCTION(stordb_fs_blchown));
  STORDB_MODULE_SET(fs, "size", V8FUNCTION(stordb_fs_bsize));
  STORDB_MODULE_SET(fs, "fsize", V8FUNCTION(stordb_fs_bfsize));
  STORDB_MODULE_SET(fs, "read", V8FUNCTION(stordb_fs_bread));
  STORDB_MODULE_SET(fs, "fread", V8FUNCTION(stordb_fs_bfread));
  STORDB_MODULE_SET(fs, "fnread", V8FUNCTION(stordb_fs_bfnread));
  STORDB_MODULE_SET(fs, "write", V8FUNCTION(stordb_fs_bwrite));
  STORDB_MODULE_SET(fs, "fwrite", V8FUNCTION(stordb_fs_bfwrite));
  STORDB_MODULE_SET(fs, "fnwrite", V8FUNCTION(stordb_fs_bfnwrite));
  STORDB_MODULE_SET(fs, "mkdir", V8FUNCTION(stordb_fs_bmkdir));
  STORDB_MODULE_SET(fs, "rmdir", V8FUNCTION(stordb_fs_brmdir));
  STORDB_MODULE_SET(fs, "exists", V8FUNCTION(stordb_fs_bexists));
  STORDB_MODULE_SET(fs, "seek", V8FUNCTION(stordb_fs_bseek));
});

void
stordb_fs_berror (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  // protect against bad arguments
  if (args.Length() < 1) {
    V8THROW("expecting two arguments");
    V8RETURN(V8NULL());
  }

  // prefix
  v8::String::Utf8Value prefix(args[0]);

  // throw
  fs_error(*prefix);
}

void
stordb_fs_bopen (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  // protect against bad arguments
  if (args.Length() < 1) {
    V8THROW("expecting two arguments");
    V8RETURN(V8NULL());
  }

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // args
  v8::String::Utf8Value path(args[0]);
  v8::String::Utf8Value flags(args[1]);

  // open
  FILE *ref = fs_open(*path, *flags);

  if (NULL == ref) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  // wrap ref
  v8::Local<v8::Object> obj = V8WRAP(ref);

  V8RETURN(obj);
}

void
stordb_fs_bclose (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("expecting two arguments");
    V8RETURN(V8FALSE());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  if (0 != fs_close(ref)) {
    V8THROW(strerror(errno));
    V8RETURN(V8FALSE());
  }

  V8RETURN(V8TRUE());
}

void
stordb_fs_brename (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8FALSE());
  }

  v8::String::Utf8Value from(args[0]);
  v8::String::Utf8Value to(args[1]);

  if (0 != fs_rename(*from, *to)) {
    V8RETURN(V8FALSE());
  }

  V8RETURN(V8TRUE());
}

void
stordb_fs_bstat (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // path
  v8::String::Utf8Value path(args[0]);

  // stat
  fs_stats *fstats = fs_stat((const char *) *path);

  if (NULL == fstats) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  // stats
  v8::Handle<v8::ObjectTemplate> stats = v8::ObjectTemplate::New(iso);

  // wrap stats
  wrap_stats(stats, fstats);

  // return new instance of stats
  V8RETURN(stats->NewInstance());
}

void
stordb_fs_bfstat (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // unwrap
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // stat
  fs_stats *fstats = fs_fstat(ref);

  if (NULL == fstats) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  // stats
  v8::Handle<v8::ObjectTemplate> stats = v8::ObjectTemplate::New(iso);

  // wrap stats
  wrap_stats(stats, fstats);

  V8RETURN(stats->NewInstance());
}

void
stordb_fs_blstat (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // path
  v8::String::Utf8Value path(args[0]);

  // stat
  fs_stats *fstats = fs_lstat(*path);

  if (NULL == fstats) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  // stats
  v8::Handle<v8::ObjectTemplate> stats = v8::ObjectTemplate::New(iso);

  // wrap stats
  wrap_stats(stats, fstats);

  V8RETURN(stats->NewInstance());
}

void
stordb_fs_bftruncate (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // len
  int len = args[1]->ToNumber()->Int32Value();

  if (0 != fs_ftruncate(ref, len)) {
    V8THROW(strerror(errno));
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_btruncate (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // len
  int len = args[1]->ToNumber()->Int32Value();

  if (0 != truncate(*path, len)) {
    V8THROW(strerror(errno));
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_bchown (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (3 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // uid
  int uid = args[1]->ToNumber()->Int32Value();

  // gid
  int gid = args[2]->ToNumber()->Int32Value();

  if (0 != fs_chown(*path, uid, gid)) {
    V8THROW(strerror(errno));
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_bfchown (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (3 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE * ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // uid
  int uid = args[1]->ToNumber()->Int32Value();

  // gid
  int gid = args[2]->ToNumber()->Int32Value();

  if (0 != fs_fchown(ref, uid, gid)) {
    V8THROW(strerror(errno));
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_blchown (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (3 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // uid
  int uid = args[1]->ToNumber()->Int32Value();

  // gid
  int gid = args[2]->ToNumber()->Int32Value();

  if (0 != fs_lchown(*path, uid, gid)) {
    V8THROW(strerror(errno));
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_bsize (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // size
  v8::Handle<v8::Number> size = V8NUMBER(fs_size((const char *) *path));

  // result
  V8RETURN(size);
}

void
stordb_fs_bfsize (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // size
  v8::Handle<v8::Number> size = V8NUMBER(fs_fsize(ref));

  // result
  V8RETURN(size);
}

void
stordb_fs_bread (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  // protect against bad arguments
  if (args.Length() < 1) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // read
  char *buf = fs_read((const char *) *path);

  // wrap and return
  V8RETURN(V8STRING(buf));
}

void
stordb_fs_bnread (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // len
  int len = args[1]->ToNumber()->Int32Value();

  // read
  char *buf = fs_nread((const char *) *path, len);

  if (NULL == buf) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8STRING(buf));
}

void
stordb_fs_bfread (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // read
  char *buf = fs_fread(ref);

  if (NULL == buf) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8STRING(buf));
}

void
stordb_fs_bfnread (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // len
  int len = args[1]->ToNumber()->Int32Value();

  // read
  char *buf = fs_fnread(ref, len);

  if (NULL == buf) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8STRING(buf));
}

void
stordb_fs_bwrite (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // buffer
  v8::String::Utf8Value buf(args[1]);

  // write
  size_t size = fs_write(*path, *buf);

  if (-1 == size) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8NUMBER(size));
}

void
stordb_fs_bfwrite (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // buffer
  v8::String::Utf8Value buf(args[1]);

  // write
  size_t size = fs_fwrite(ref, *buf);

  if (-1 == size) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8NUMBER(size));
}

void
stordb_fs_bfnwrite (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (3 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // buffer
  v8::String::Utf8Value buf(args[1]);

  // len
  int len = args[2]->ToNumber()->Int32Value();

  // write
  size_t size = fs_fnwrite(ref, *buf, len);

  if (-1 == size) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8NUMBER(size));
}

void
stordb_fs_bmkdir (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // mode
  int mode = args[1]->ToNumber()->Int32Value();

  // mkdir
  if (-1 == fs_mkdir(*path, mode)) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_brmdir (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // rmdir
  if (-1 == fs_rmdir(*path)) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8NULL());
}

void
stordb_fs_bexists (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // exists?
  if (-1 == fs_exists(*path)) {
    V8RETURN(V8FALSE());
  }

  V8RETURN(V8TRUE());
}

void
stordb_fs_bseek (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (3 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // ref
  FILE *ref = (FILE *) V8UNWRAP(args[0]->ToObject());

  // offset
  int offset = args[1]->ToNumber()->Int32Value();

  // whence
  int whence = args[2]->ToNumber()->Int32Value();

  if (-1 == fseek(ref, offset, whence)) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8NULL());
}
