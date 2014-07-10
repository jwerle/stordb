
#ifndef STORDB_H
#define STORDB_H

#include <leveldb/db.h>
#include <v8.h>

#ifndef STORDB_JS_PATH
#error Missing path to `stordb.js'. Please define `STORDB_JS_PATH'
#endif

typedef struct {
  char **env;

  // v8
  struct {
    int safe_:1;
    v8::Isolate *iso;
    v8::Handle<v8::Context> ctx;
    v8::Handle<v8::ObjectTemplate> global;
  } v8;
} stordb_t;

/**
 * API
 */

int
stordb_initialize (stordb_t *, char **);

stordb_t *
stordb_get_current ();

#endif

