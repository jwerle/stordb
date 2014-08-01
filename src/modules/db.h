
#ifndef STORDB_DB_H
#define STORDB_DB_H

#include <stdlib.h>
#include <v8.h>
#include <leveldb/db.h>
#include "stordb.h"

typedef struct {
  const char *path;
  //void *dbh;
  leveldb::DB *dbh;
} stordb_db_t;

void
stordb_db_bnew (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_db_bget (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_db_bput (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_db_bdel (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_db_bfind (const v8::FunctionCallbackInfo<v8::Value> &);

stordb_db_t *
stordb_db_new (stordb_t *, const char *);

#endif
