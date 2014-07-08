
#include <stdio.h>
#include <v8.h>
#include "js.h"
#include "stordb.h"

extern "C" {
#include <fs/fs.h> 
}

static int
_initialize_v8 (stordb_t *);

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
  const char *js = fs_read(STORDB_JS_PATH);
  int rc = 0;
  sdb->v8.iso = v8::Isolate::New();
  sdb->v8.global = v8::ObjectTemplate::New();
  sdb->v8.ctx = v8::Context::New(NULL, sdb->v8.global);
  rc = stordb_runjs(sdb, "stordb", js);
  if (1 == rc) { return 1; }
  return 0;
}

