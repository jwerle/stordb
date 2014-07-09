
#include <stdio.h>
#include <v8.h>

#include "stordb.h"
#include "js.h"
#include "bindings.h"

extern "C" {
#include <fs/fs.h>
#include <asprintf/asprintf.h>
}

static char *SCRIPTS[] = {
#define X(s) (char *) s
  X("database.js"),
  X("stordb.js"),
  NULL
#undef X
};

static int
_initialize_v8 (stordb_t *);

static int
_initialize_v8_bindings (stordb_t *);

int
stordb_initialize (stordb_t *sdb) {
  int rc = 0;
  rc = _initialize_v8(sdb);
  if (1 == rc) { return 1; }
  return 0;
}

static int
_initialize_v8 (stordb_t *sdb) {
  if (NULL == sdb) { return 1; }
  int rc = 0;

  // scope
  v8::Isolate *iso = v8::Isolate::GetCurrent();
  v8::HandleScope hscope(iso);
  // instance
  sdb->v8.iso = iso;

  // global
  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(iso);
  sdb->v8.global = global;

  // bindings
  rc = _initialize_v8_bindings(sdb);

  // context
  v8::Handle<v8::Context> ctx = v8::Context::New(iso, NULL, global);
  v8::Context::Scope context_scope(ctx);
  sdb->v8.ctx = ctx;

  // bootstrap
  ctx->Enter();
  {
    v8::HandleScope handle_scope(iso);
    int i = 0;
    char *js = NULL;
    for (; SCRIPTS[i]; ++i) {
      char *path = NULL;
      asprintf(&path, "%s/%s", STORDB_JS_PATH, SCRIPTS[i]);
      if (NULL == path) { return 1; }
      if (-1 == fs_exists(path)) {
        fprintf(stderr, "error: script `%s' doesn't exists\n", path);
        return 1;
      }
      js = fs_read(path);
      if (NULL == js) {
        free(path);
        return 1;
      }
      rc = stordb_runjs(sdb, path, js);
      if (1 == rc) {
        free(path);
        return 1;
      }

      free(path);
      free(js);
    }
  }
  ctx->Exit();

  return 0;
}

static int
_initialize_v8_bindings (stordb_t *sdb) {
  if (NULL == sdb) { return 1; }
  v8::Handle<v8::ObjectTemplate> global = sdb->v8.global;
#define XF(k, v) \
  global->Set(v8::String::NewFromUtf8(sdb->v8.iso, k), \
      v8::FunctionTemplate::New(sdb->v8.iso, v));

  XF("print", stordb_bprint);

#undef XF


  return 0;
}
