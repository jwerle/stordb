
#include "stordb.h"
#include "stordb/macro.h"
#include "stordb/module.h"
#include "io.h"

extern "C" {
}

STORDB_MODULE(io, {
  STORDB_MODULE_SET(io, "stdin", V8NUMBER(0));
  STORDB_MODULE_SET(io, "stdout", V8NUMBER(1));
  STORDB_MODULE_SET(io, "stderr", V8NUMBER(2));
  STORDB_MODULE_SET(io, "read", V8FUNCTION(stordb_io_read));
  STORDB_MODULE_SET(io, "write", V8FUNCTION(stordb_io_write));
});

void
stordb_io_read (const v8::FunctionCallbackInfo<v8::Value> &args) {
  char buf[BUFSIZ];
  int fd = 0;
  size_t size = 0;
  ssize_t bytes = 0;

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope scope(iso);

  // arg value
  v8::Handle<v8::Value> arg;

  if (2 != args.Length()) {
    return args.GetReturnValue().Set(v8::Null(iso));
  }

  // fd
  arg = args[0];
  fd = arg->ToNumber()->Int32Value();

  // size
  arg = args[1];
  size = arg->ToNumber()->Int32Value();

  // read
  bytes = read(fd, buf, size);
  buf[bytes] = '\0';

  if (0 == bytes) {
    return args.GetReturnValue().Set(v8::Null(iso));
  } else {
    return args.GetReturnValue().Set(V8STRING(buf));
  }
}

void
stordb_io_write (const v8::FunctionCallbackInfo<v8::Value> &args) {
  char *buf = NULL;
  int fd = 0;
  size_t size = 0;
  ssize_t bytes = 0;

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope scope(iso);

  // arg value
  v8::Handle<v8::Value> arg;

  if (3 != args.Length()) {
    return args.GetReturnValue().Set(v8::Null(iso));
  }

  // fd
  arg = args[0];
  fd = arg->ToNumber()->Int32Value();

  // buf
  arg = args[1];
  v8::String::Utf8Value vbuf(arg);
  buf = *vbuf;

  // size
  arg = args[2];
  size = arg->ToNumber()->Int32Value();

  // write
  bytes = write(fd, buf, size);

  if (0 == bytes) {
    return args.GetReturnValue().Set(v8::False(iso));
  } else {
    return args.GetReturnValue().Set(v8::True(iso));
  }
}
