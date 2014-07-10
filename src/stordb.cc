
#include <stdio.h>
#include <v8.h>

#include "stordb.h"
#include "js.h"
#include "db.h"
#include "bindings.h"

extern "C" {
#include <fs/fs.h>
#include <asprintf/asprintf.h>
}

static stordb_t *gsdb_ = NULL;

#define X(s) (char *) s
static char *BOOT_SCRIPTS[] = {
  X("env.js"),
};

static char *SCRIPTS[] = {
  X("vm.js"),
  X("database.js"),
  X("stordb.js"),
  NULL
};
#undef X

static int
_initialize_v8 (stordb_t *, char **);

static int
_initialize_v8_bindings (stordb_t *);

static int
_set_env (stordb_t *, char **);

int
stordb_initialize (stordb_t *sdb, char **env) {
  int rc = 0;
  rc = _initialize_v8(sdb, env);
  if (1 == rc) { return 1; }
  gsdb_ = sdb;
  return 0;
}

stordb_t *
stordb_get_current () {
  return gsdb_;
}

static int
_set_env (stordb_t *sdb, char **env) {
  char *line = NULL;
  char key[BUFSIZ];
  char value[BUFSIZ];
  char tmp[BUFSIZ];
  char ch = 0;
  int i = 0;
  int body = 0;
  size_t size = 0 ;
  sdb->env = env;

  v8::Isolate::Scope isolate_scope(sdb->v8.iso);
  v8::HandleScope hscope(sdb->v8.iso);
  v8::Handle<v8::ObjectTemplate> ENV = v8::ObjectTemplate::New();

  while ((line = *env++)) {
    i = 0;
    size = 0;

    memset(tmp, 0, BUFSIZ);
    memset(key, 0, BUFSIZ);
    memset(value, 0, BUFSIZ);

    do {
      ch = line[i];
      if ('=' == ch) {
        // set key
        strncat(key, tmp, size);
        key[size] = '\0';
        size = 0;
      } else {
        tmp[size++] = ch;
      }
    } while ('\0' != (ch = line[++i]));

    // set value
    strncat(value, tmp, size);
    value[size] = '\0';

    // set in v8
    ENV->Set(
        v8::String::NewFromUtf8(sdb->v8.iso, key),
        v8::String::NewFromUtf8(sdb->v8.iso, value));
  }

  sdb->v8.global->Set(
      v8::String::NewFromUtf8(sdb->v8.iso, "ENV"), ENV);

  return 0;
}

static int
_initialize_v8 (stordb_t *sdb, char **env) {
  if (NULL == sdb) { return 1; }
  int rc = 0;

  // mark unsafe
  sdb->v8.safe_ = 0;

  // scope
  v8::Isolate *iso = v8::Isolate::GetCurrent();
  // handle `NULL' isolates
  if (NULL == iso) {
    iso = v8::Isolate::New();
  }
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope hscope(iso);
  // instance
  sdb->v8.iso = iso;

  // global
  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(iso);
  sdb->v8.global = global;

  // bindings
  rc = _initialize_v8_bindings(sdb);

  // env
  rc = _set_env(sdb, env);

  // context
  v8::Handle<v8::Context> ctx = v8::Context::New(iso, NULL, global);
  v8::Context::Scope context_scope(ctx);
  sdb->v8.ctx = ctx;

  // bootstrap
  ctx->Enter();
  {
    v8::Handle<v8::Value> value;
    int i = 0;
    char *js = NULL;
    char *path = NULL;

    // boot scripts
    {
      char BSBUF[BUFSIZ];
      size_t size = 0;
      memset(BSBUF, 0, BUFSIZ);
      for (; BOOT_SCRIPTS[i]; ++i) {
        // boot script path
        path = NULL;
        asprintf(&path, "%s/%s", STORDB_JS_PATH, BOOT_SCRIPTS[i]);
        if (NULL == path) { return 1; }
        if (-1 == fs_exists(path)) {
          fprintf(stderr,
                  "error: (BOOT) boot script `%s' doesn't exists\n",
                  path);
          return 1;
        }

        // read
        js = fs_read(path);
        if (NULL == js) { return free(path), 1; }

        // append to buffer
        strcat(BSBUF, js);

        // size
        size += strlen(js);

        // free
        free(path);
        free(js);
      }

      BSBUF[size] = '\0';

      // exec
      value = stordb_runjs(sdb, "[boot]", BSBUF);
      if (v8::Null(sdb->v8.iso) == value) { return 1; }
    }

    // app scripts
    {
      char BSBUF[BUFSIZ];
      size_t size = 0;
      memset(BSBUF, 0, BUFSIZ);
      for (i = 0; SCRIPTS[i]; ++i) {
        // script path
        path = NULL;
        asprintf(&path, "%s/%s", STORDB_JS_PATH, SCRIPTS[i]);
        if (NULL == path) { return 1; }
        if (-1 == fs_exists(path)) {
          fprintf(stderr, "error: script `%s' doesn't exists\n", path);
          return 1;
        }

        // read
        js = fs_read(path);
        if (NULL == js) { return free(path), 1; }

        // append to buffer
        strcat(BSBUF, js);

        // size
        size += strlen(js);

        // free
        free(path);
        free(js);
      }

      BSBUF[size] = '\0';

      // exec
      value = stordb_runjs(sdb, "[stordb]", BSBUF);
      if (v8::Null(sdb->v8.iso) == value) { return 1; }
    }
  }

  ctx->Exit();

  // mark safe
  sdb->v8.safe_ = 1;
  return 0;
}

static int
_initialize_v8_bindings (stordb_t *sdb) {
  if (NULL == sdb) { return 1; }
  v8::Handle<v8::ObjectTemplate> global = sdb->v8.global;

// functions
#define XF(k, v) \
  global->Set(v8::String::NewFromUtf8(sdb->v8.iso, k), \
      v8::FunctionTemplate::New(sdb->v8.iso, v));

// numbers
#define XI(k, v) \
  global->Set(v8::String::NewFromUtf8(sdb->v8.iso, k), \
      v8::Number::New(sdb->v8.iso, v));

  // io fds
  XI("stdin", 0);
  XI("stdout", 1);
  XI("stderr", 2);

  // util
  XF("print", stordb_bprint);

  // db
  XF("__ldb_new", stordb_db_bnew);
  XF("__ldb_get", stordb_db_bget);
  XF("__ldb_put", stordb_db_bput);
  XF("__ldb_del", stordb_db_bdel);

#undef XF
#undef XI

  return 0;
}
