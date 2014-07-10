
#include <stdio.h>
#include <stdlib.h>
#include <v8.h>

#include "stordb.h"

int
main (int argc, char **argv, char **env) {
  // v8 initialization
  {
    argv[argc++] = (char *) "--harmony";
    v8::V8::InitializeICU();
    v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
  }

  // stordb initialization
  {
    int rc = 0;
    stordb_t *sdb = (stordb_t *) malloc(sizeof(stordb_t));
    if (NULL == sdb) { return 1; }
    rc = stordb_initialize(sdb, env);
    if (1 == rc) { return 1; }
  }

  // run any files passed in

  v8::V8::Dispose();
  return 0;
}
