
#ifndef STORDB_MACRO_H
#define STORDB_MACRO_H

#include <v8.h>

#define SDBV8H(f, v) f(v8::Isolate::GetCurrent(), v)
#define V8STRING(v) SDBV8H(v8::String::NewFromUtf8, v)
#define V8FUNCTION(v) SDBV8H(v8::FunctionTemplate::New, v)
#define V8NUMBER(v) SDBV8H(v8::Number::New, v)

#define STORDB_SET_BINDING(ns, key, value) do {                                \
  char *tmp = NULL;                                                            \
  asprintf(&tmp, "__%s_%s__", # ns, key);                                      \
  stordb_get_current()->v8.global->Set(V8STRING(tmp), value);                  \
  free(tmp);                                                                   \
} while (0);

#endif
