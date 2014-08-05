
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

  // path
  v8::String::Utf8Value path(args[0]);
  printf("%s\n", *path);
  typedef void (*init_t)(v8::Handle<v8::ObjectTemplate>);
  void *ld = dlopen(*path, RTLD_NOW);
  if (NULL == ld) {
    V8THROW(dlerror());
  }

  init_t Init = (init_t) dlsym(ld, "Init");
  if (NULL == Init) {
    V8THROW(dlerror());
  }

  // exports
  v8::Handle<v8::ObjectTemplate> exports = v8::ObjectTemplate::New(args.GetIsolate());
  Init(exports);
  V8RETURN(V8NULL());
}
