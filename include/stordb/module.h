
#ifndef STORDB_MODULE_H
#define STORDB_MODULE_H

#include "stordb.h"
#include "macro.h"

#define STORDB_MODULE(mod, body) \
  void stordb_module__ ## mod (void) { \
    { \
      stordb_get_current()->v8.modules->Set(V8STRING(# mod), \
          v8::ObjectTemplate::New(stordb_get_current()->v8.iso)); \
      body; \
    } \
  } \

#define STORDB_MODULE_INIT(mod) stordb_module__ ## mod ();
#define STORDB_MODULE_DECLARE(mod) void stordb_module__ ## mod ();

#define STORDB_MODULE_SET(mod, key, value) do {                                \
  char *tmp = NULL;                                                            \
  asprintf(&tmp, "%s_%s", # mod, key);                                         \
  stordb_get_current()->v8.modules->Set(V8STRING(tmp), value);                 \
  free(tmp);                                                                   \
} while(0); \

#endif
