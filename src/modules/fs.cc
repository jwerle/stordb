

#include "stordb.h"
#include "fs.h"

extern "C" {
#include <fs/fs.h>
}

void
stordb_fs_bread (const v8::FunctionCallbackInfo<v8::Value> &args) {
  const char *path = NULL;
  char *buf = NULL;
  v8::Handle<v8::Value> arg;

  // isolate
  v8::Isolate *iso = v8::Isolate::GetCurrent();

  // scope
  v8::HandleScope scope(args.GetIsolate());

  // protect against bad arguments
  if (args.Length() < 1) {
    return args.GetReturnValue().Set(v8::Null(args.GetIsolate()));
  }

  // path
  arg = args[0];
  v8::String::Utf8Value vpath(arg);
  path = *vpath;

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // read
  buf = fs_read(path);

  // wrap and return
  args.GetReturnValue().Set(v8::String::NewFromUtf8(iso, buf));
}

void
stordb_fs_bnread (const v8::FunctionCallbackInfo<v8::Value> &args) {

}

void
stordb_fs_bwrite (const v8::FunctionCallbackInfo<v8::Value> &args) {

}

void
stordb_fs_bnwrite (const v8::FunctionCallbackInfo<v8::Value> &args) {

}

void
stordb_fs_bmkdir (const v8::FunctionCallbackInfo<v8::Value> &args) {

}

void
stordb_fs_bexists (const v8::FunctionCallbackInfo<v8::Value> &args) {

}

void
stordb_fs_brename (const v8::FunctionCallbackInfo<v8::Value> &args) {

}

void
stordb_fs_bstat (const v8::FunctionCallbackInfo<v8::Value> &args) {

}
