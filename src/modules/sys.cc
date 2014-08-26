
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <time.h>
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

STORDB_MODULE(sys, {
  STORDB_MODULE_SET(sys, "exit", V8FUNCTION(stordb_sys_exit));
  STORDB_MODULE_SET(sys, "load", V8FUNCTION(stordb_sys_load));
  STORDB_MODULE_SET(sys, "print", V8FUNCTION(stordb_sys_print));
  STORDB_MODULE_SET(sys, "cwd", V8FUNCTION(stordb_sys_cwd));
  STORDB_MODULE_SET(sys, "chdir", V8FUNCTION(stordb_sys_chdir));
  STORDB_MODULE_SET(sys, "access", V8FUNCTION(stordb_sys_access));
  STORDB_MODULE_SET(sys, "system", V8FUNCTION(stordb_sys_system));

  STORDB_MODULE_SET(sys, "R_OK", V8NUMBER(R_OK));
  STORDB_MODULE_SET(sys, "F_OK", V8NUMBER(F_OK));
  STORDB_MODULE_SET(sys, "W_OK", V8NUMBER(W_OK));
})

static char *
wrap (char *path, char *src) {
  char ch = 0;
  char *wrapped = NULL;
  char *key = NULL;

  asprintf(&key, "_s%ld", time(NULL) * rand());

  // drop #!
  if ('#' == src[0]) {
    while ((ch = *src++)) if ('\n' == ch) { break; }
  }

  asprintf(&wrapped, "module %s {"
        // commonjs interface
        "export var module = new Module('%s', module);"
        "var exports = module.exports;"
        "let __filename = module.filename;"
        "let __dirname = module.dirname;"
        "function require (path) {"
          "return module.require(path);"
        "}"

        // module source
        "%s"

        // unwrap into `module.exports'
        "{"
          "let _;"
          "for (_ in %s) {"
            "if ('module' != _) {"
              "module.exports[_] = %s[_];"
            "}"
          "}"
        "}"
      "}"

      // return module
      "%s.module",
    key, path,
    src, key, key, key);

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
  if (0 == fs_exists(resolved)) {  return resolved; }
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
  V8SCOPE(args);
  v8::HandleScope scope(args.GetIsolate());
  v8::String::Utf8Value str(args[0]);
  printf("%s\n", *str);
  fflush(stdout);
}

void
stordb_sys_load (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);
  char *orig = NULL;
  char *path = NULL;
  char *buf = NULL;
  char *err = NULL;
  int resolved = 0;

  // protect against bad arguments
  if (args.Length() < 1) {
    V8RETURN(V8NULL());
  }

  // path
  v8::String::Utf8Value vpath(args[0]);
  path = *vpath;

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  if (0 == strcmp("__native_bindings__", path)) {
    V8RETURN(sdb->v8.modules->NewInstance());
  }

  // resolve
  orig = strdup(path);
  path = resolve(path);

  // handle not found
  if (NULL == path) {
    asprintf(&err, "ENOENT: `%s' not found", orig);
    V8THROW(err);
    free(err);
    V8RETURN(V8NULL());
  }

  // read
  buf = fs_read(path);
  if (NULL == buf) {
    V8RETURN(V8NULL());
  }

  // module wrap
  char *mod = wrap(path, buf);

  // run
  v8::Handle<v8::Value> result = stordb_runjs(sdb, path, mod);
  free(mod);

  // return
  V8RETURN(result);
}

void
stordb_sys_exit (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);
  int rc = 0;

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
  V8SCOPE(args);
  char cwd[1024];
  if (NULL == getcwd(cwd, sizeof(cwd))) {
    V8THROW(strerror(errno));
    V8RETURN(V8NULL());
  }

  V8RETURN(V8STRING(cwd));
}

void
stordb_sys_chdir (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);
  v8::String::Utf8Value path(args[0]);
  if (0 != chdir(*path)) {
    V8THROW(strerror(errno));
  }
}

void
stordb_sys_access (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  if (2 != args.Length()) {
    V8RETURN(V8FALSE());
  }

  // path
  v8::String::Utf8Value path(args[0]);

  // mode
  v8::Handle<v8::Number> mode = args[1]->ToNumber();

  if (0 != access(*path, mode->Int32Value())) {
    V8THROW(strerror(errno));
    V8RETURN(V8FALSE());
  }

  V8RETURN(V8TRUE());
}

void
stordb_sys_system (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  // command
  v8::String::Utf8Value cmd(args[0]);

  // run
  v8::Handle<v8::Number> rc = V8NUMBER(system(*cmd));

  // return exit code
  V8RETURN(rc);
}
