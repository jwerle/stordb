
#include <v8.h>
#include <stordb.h>
#include <stordb/module.h>

extern "C" void
Init (v8::Handle<v8::ObjectTemplate> exports) {
  printf("foo\n");
}

