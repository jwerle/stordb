
#include <v8.h>
#include "stordb.h"
#include "js.h"

int
stordb_runjs (stordb_t *sdb, const char *name, const char *src) {
  // scope
  v8::Isolate::Scope isolate_scope(sdb->v8.iso);
  v8::HandleScope handle_scope(sdb->v8.iso);

  // context
  v8::Context::Scope context_scope(sdb->v8.ctx);

  // script
  v8::Handle<v8::String> v8src = v8::String::NewFromUtf8(sdb->v8.iso, src);
  v8::TryCatch tc;
  v8::Handle<v8::String> v8name = v8::String::NewFromUtf8(sdb->v8.iso, name);

  // compile
  v8::ScriptOrigin origin(v8name);
  v8::Handle<v8::Script> script = v8::Script::Compile(v8src, &origin);
  if (script.IsEmpty()) { return 1; }

  // run
  v8::Handle<v8::Value> result = script->Run();

  return 0;
}
