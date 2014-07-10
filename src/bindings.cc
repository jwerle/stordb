
#include <stdio.h>
#include <stdlib.h>
#include <v8.h>

#include "bindings.h"

void
stordb_bprint (const v8::FunctionCallbackInfo<v8::Value> &arguments) {
  v8::HandleScope scope(arguments.GetIsolate());
  v8::String::Utf8Value str(arguments[0]);
  printf("%s\n", *str);
  fflush(stdout);
}
