
#ifndef STORDB_H
#define STORDB_H

#include <leveldb/db.h>
#include <v8.h>

#ifndef STORDB_JS_PATH
#error Missing path to `stordb.js'. Please define `STORDB_JS_PATH'
#endif

/**
 * Data structures
 */

typedef struct {
  // leveldb
  leveldb::DB *ldb;

  // v8
  struct {
    v8::Isolate *iso;
    v8::Handle<v8::Context> ctx;
    v8::Handle<v8::ObjectTemplate> global;
  } v8;
} stordb_t;


/**
 * API
 */

int
stordb_initialize (stordb_t *);

#endif
