
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>

#include "stordb.h"
#include "stordb/macro.h"
#include "stordb/module.h"

#include "ld.h"

STORDB_MODULE(ld, {
  STORDB_MODULE_SET(ld, "open", V8FUNCTION(stordb_ld_bopen));
});

void
stordb_ld_bopen (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);
  if (1 != args.Length()) {
    V8THROW("Invalid arguments");
    V8RETURN(V8NULL());
  }

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // path
  v8::String::Utf8Value path(args[0]);

  // init cb
  typedef v8::Handle<v8::Value> (*init_t)(v8::Isolate *, v8::Handle<v8::ObjectTemplate>);

  // dl open
  void *ld = dlopen(*path, RTLD_NOW|RTLD_LOCAL);
  if (NULL == ld) {
    V8THROW(dlerror());
  }

  // exports
  v8::Handle<v8::ObjectTemplate> exports = v8::ObjectTemplate::New(iso);

  // `Init' sym
  init_t Init = (init_t) dlsym(ld, "Init");
  if (NULL == Init) {
    V8THROW(dlerror());
  }

  // init
  {
    v8::Isolate::Scope isolate_scope(iso);
    Init(iso, exports);
  }
  V8RETURN(exports->NewInstance());
}
