
#ifndef STORDB_H
#define STORDB_H

#include <v8.h>

#ifndef STORDB_JS_PATH
#error Missing path to `stordb.js'. Please define `-DSTORDB_JS_PATH="/path"'
#endif

#define STORDB_VERSION "0.0.1"

typedef struct {
  int argc;
  char **argv;
  char **env;

  // v8
  struct {
    int safe_:1;
    v8::Isolate *iso;
    v8::Handle<v8::Context> ctx;
    v8::Handle<v8::ObjectTemplate> global;
    v8::Handle<v8::ObjectTemplate> modules;
  } v8;
} stordb_t;

/**
 * API
 */

int
stordb_initialize (stordb_t *, int, char **, char **);

stordb_t *
stordb_get_current ();

#include "stordb/js.h"
#include "stordb/macro.h"
#include "stordb/module.h"
#include "stordb/exception.h"

STORDB_MODULE_DECLARE(STORDB);

#endif

