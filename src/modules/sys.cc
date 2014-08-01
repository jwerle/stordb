
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <v8.h>

#include "stordb.h"
#include "stordb/exception.h"
#include "stordb/js.h"

#include "sys.h"

extern "C" {
#include <strsplit/strsplit.h>
#include <asprintf/asprintf.h>
#include <extname/extname.h>
#include <isdir/isdir.h>
#include <fs/fs.h>
}

static char *
wrap (char *src) {
  char *wrapped = NULL;
  char *key = NULL;
  asprintf(&key, "__sandbox__%ld", time(NULL) * rand());
  asprintf(&wrapped, ""
      "var module = {exports: {}};"
      "var exports = module.exports;"
      "module %s {"
        "export function __unwrap__ () {"
          "for (var prop in %s) {"
            "if ('__unwrap__' != prop) { module.exports[prop] = %s[prop]; }"
          "}"
        "}"
        "%s"
      "}"
       "%s.__unwrap__(); module;",
       key, key, key, src, key);

  free(key);
  return wrapped;
}

static char *
resolve (char *path) {
  char *PATH = getenv("STORDB_PATH");
  char *tmp = NULL;
  char *resolved = NULL;
  char *paths[16];
  size_t size = 0;
  int i = 0;
  int needs_free = 0;
  resolved = path;
  if (0 == fs_exists(resolved)) { return resolved; }
  if (NULL != PATH) {
    size = strsplit(PATH, paths, ":");
  } else {
    paths[size++] = (char *) STORDB_JS_PATH;
  }
  if (0 == strcmp(extname(resolved), "")) {
    if (0 == isdir(resolved)) {
      asprintf(&resolved, "%s/index.js", resolved);
    } else {
      asprintf(&resolved, "%s.js", resolved);
    }
    needs_free = 1;
  } else if (0 == isdir(resolved)) {
    asprintf(&resolved, "%s/index.js", resolved);
    needs_free = 1;
  }

  if (0 == fs_exists(resolved)) { return resolved; }
  for (;i < size; ++i) {
    free(tmp);
    tmp = NULL;
    asprintf(&tmp, "%s/%s", paths[i], resolved);
    if (0 == isdir(resolved)) {
      asprintf(&tmp, "%s/index.js", tmp);
    }

    if (0 == fs_exists(tmp)) {
      if (needs_free) { free(resolved); }
      return tmp;
    }
  }

  return NULL;
}

void
stordb_sys_print (const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::HandleScope scope(args.GetIsolate());
  v8::String::Utf8Value str(args[0]);
  printf("%s\n", *str);
  fflush(stdout);
}

void
stordb_sys_load (const v8::FunctionCallbackInfo<v8::Value> &args) {
  char *orig = NULL;
  char *path = NULL;
  char *buf = NULL;
  char *err = NULL;
  int resolved = 0;

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope scope(iso);

  // arg value
  v8::Handle<v8::Value> arg;

  // protect against bad arguments
  if (args.Length() < 1) {
    return args.GetReturnValue().Set(v8::Null(iso));
  }

  // path
  arg = args[0];
  v8::String::Utf8Value vpath(arg);
  path = *vpath;

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // resolve
  orig = path;
  path = resolve(path);

  // handle not found
  if (NULL == path) {
    asprintf(&err, "ENOENT: `%s' not found", orig);
    iso->ThrowException(v8::String::NewFromUtf8(iso, err));
    free(err);
    return args.GetReturnValue().Set(v8::Null(iso));
  }

  // read
  buf = fs_read(path);
  if (NULL == buf) { return args.GetReturnValue().Set(v8::Null(iso)); }

  // module wrap
  char *mod = wrap(buf);

  // run
  v8::Handle<v8::Value> result = stordb_runjs(sdb, path, mod);
  free(mod);

  // return
  args.GetReturnValue().Set(result);
}

void
stordb_sys_exit (const v8::FunctionCallbackInfo<v8::Value> &args) {
  int rc = 0;

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope scope(iso);

  // arg value
  v8::Handle<v8::Value> arg = args[0];

  // dispose
  v8::V8::Dispose();

  if (arg->IsUndefined() || arg->IsNull()) {
    exit(0);
  }

  arg = args[0];
  v8::Handle<v8::Number> code = arg->ToNumber();
  rc = (int) code->Int32Value();
  exit(rc);
}

void
stordb_sys_cwd (const v8::FunctionCallbackInfo<v8::Value> &args) {
  char cwd[1024];

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope scope(iso);
  if (NULL == getcwd(cwd, sizeof(cwd))) {
    iso->ThrowException(v8::String::NewFromUtf8(iso, strerror(errno)));
    return;
  }
  args.GetReturnValue().Set(V8STRING(cwd));
}

void
stordb_sys_chdir (const v8::FunctionCallbackInfo<v8::Value> &args) {
  char *path = NULL;
  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope scope(iso);

  // arg value
  v8::Handle<v8::Value> arg = args[0];
  v8::String::Utf8Value vpath(arg);
  path = *vpath;

  int rc = chdir(path);
  if (0 != rc) {
    iso->ThrowException(v8::String::NewFromUtf8(iso, strerror(errno)));
  }
}
