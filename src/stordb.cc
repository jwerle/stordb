
#include <stdio.h>
#include <v8.h>

#include "stordb.h"
#include "stordb/macro.h"
#include "stordb/module.h"
#include "stordb/js.h"

#include "modules/sys.h"
#include "modules/io.h"
#include "modules/db.h"
#include "modules/fs.h"

extern "C" {
#include <fs/fs.h>
#include <asprintf/asprintf.h>
}

#define BUFMAX 4096

#define xfree(v) ({ v ? (free(v), 0) : 0; })

static stordb_t *gsdb_ = NULL;

#define X(s) (char *) s
static char *BOOT_SCRIPTS[] = {
  X("boot.js"),
  NULL
};

#undef X

static int
_initialize_v8 (stordb_t *, int, char **, char **);

static int
_initialize_v8_bindings (stordb_t *);

static int
_set_env (stordb_t *);

static int
_set_argv (stordb_t *);

STORDB_MODULE(STORDB, {
  // constants
  STORDB_MODULE_SET(STORDB, "LIB_PATH", V8STRING(STORDB_JS_PATH));
  STORDB_MODULE_SET(STORDB, "VERSION", V8STRING(STORDB_VERSION));

});

int
stordb_initialize (stordb_t *sdb, int argc, char **argv, char **env) {
  int rc = 0;
  gsdb_ = sdb;
  rc = _initialize_v8(sdb, argc, argv, env);
  if (1 == rc) { return 1; }
  return 0;
}

stordb_t *
stordb_get_current () {
  return gsdb_;
}

static int
_set_env (stordb_t *sdb) {
  char **env = sdb->env;
  char *line = NULL;
  char key[BUFMAX];
  char value[BUFMAX];
  char tmp[BUFMAX];
  char out[BUFMAX];
  char ch = 0;
  int j = 0;
  int i = 0;
  int body = 0;
  size_t size = 1 ;
  sdb->env = env;

  v8::Isolate *iso = v8::Isolate::GetCurrent();
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope hscope(iso);
  v8::Handle<v8::ObjectTemplate> ENV = v8::ObjectTemplate::New(iso);

  // init
  memset(out, 0, BUFMAX);

  // parse
  while ((line = env[j++])) {
    i = 0;
    size = 0;

    memset(tmp, 0, BUFMAX);
    memset(key, 0, BUFMAX);
    memset(value, 0, BUFMAX);

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

    ENV->Set(v8::String::NewFromUtf8(iso, key),
             v8::String::NewFromUtf8(iso, value));
  }
  sdb->v8.global->Set(v8::String::NewFromUtf8(iso, "ENV"), ENV);

  return 0;
}

static int
_set_argv (stordb_t *sdb) {
  int i = 0;
  int argc = 0;
  char str[1024];
  char **argv = sdb->argv;
  char *arg = NULL;

  // isolate/scope
  v8::Isolate *iso = v8::Isolate::GetCurrent();
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope hscope(iso);

  // build str
  memset(str, 0, 1024);
  while ((arg = argv[i++])) {
    strcat(str, arg);
    strcat(str, "!!");
  }

  // set `__ARGV__' string
  v8::Handle<v8::String> ARGV = v8::String::NewFromUtf8(iso, str);
  sdb->v8.global->Set(v8::String::NewFromUtf8(iso, "__ARGV__"), ARGV);
  return 0;
}

static int
_initialize_v8 (stordb_t *sdb, int argc, char **argv, char **env) {
  if (NULL == sdb) { return 1; }
  int rc = 0;

  // argc/argv
  sdb->argc = argc;
  sdb->argv = argv;

  // env
  sdb->env = env;

  // mark unsafe
  sdb->v8.safe_ = 0;

  // isolate
  v8::Isolate *iso = v8::Isolate::GetCurrent();
  // handle `NULL' isolates
  if (NULL == iso) { iso = v8::Isolate::New(); }

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::HandleScope hscope(iso);
  // instance
  sdb->v8.iso = iso;

  // global
  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(iso);
  sdb->v8.global = global;

  // native modules
  v8::Handle<v8::ObjectTemplate> modules = v8::ObjectTemplate::New(iso);
  sdb->v8.modules = modules;

  // bindings
  rc = _initialize_v8_bindings(sdb);

  // env
  rc = _set_env(sdb);

  // env
  rc = _set_argv(sdb);

  // context
  v8::Handle<v8::Context> ctx = v8::Context::New(iso, NULL, global);
  v8::Context::Scope context_scope(ctx);
  sdb->v8.ctx = ctx;


  // bootstrap
  ctx->Enter();

  {
    v8::Handle<v8::Value> value;
    int i = 0;

    // boot scripts
    do {
      size_t size = 0;
      char *path = NULL;
      char *js = NULL;
      for (; BOOT_SCRIPTS[i]; ++i) {
        // boot script path
        path = NULL;
        asprintf(&path, "%s/%s", STORDB_JS_PATH, BOOT_SCRIPTS[i]);
        if (NULL == path) { return 1; }
        if (-1 == fs_exists(path)) {
          fprintf(stderr, "error: (BOOT) boot script `%s' doesn't exists\n",
                  path);
          return 1;
        }

        // read
        js = fs_read(path);
        if (NULL == js) { return xfree(path), 1; }

        // run
        value = stordb_runjs(sdb, path, js);

        // free
        xfree(path);
        xfree(js);
      }

      // exec
    } while (0);
  }

  ctx->Exit();

  // mark safe
  sdb->v8.safe_ = 1;
  return 0;
}

static int
_initialize_v8_bindings (stordb_t *sdb) {
  if (NULL == sdb) { return 1; }

  // boot helpers
  STORDB_SET_BINDING("__native_require__", V8FUNCTION(stordb_sys_load));
  STORDB_SET_BINDING("__native_print__", V8FUNCTION(stordb_sys_print));
  STORDB_SET_BINDING("__native_exit__", V8FUNCTION(stordb_sys_exit));

  STORDB_MODULE_INIT(STORDB);
  STORDB_MODULE_INIT(sys);
  STORDB_MODULE_INIT(io);
  STORDB_MODULE_INIT(fs);
  STORDB_MODULE_INIT(db);


  return 0;
}
