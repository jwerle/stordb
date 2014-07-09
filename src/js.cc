
#include <v8.h>
#include "stordb.h"
#include "exception.h"
#include "js.h"

int
stordb_runjs (stordb_t *sdb, const char *name, const char *src) {
  v8::HandleScope handle_scope(sdb->v8.iso);

  // script
  v8::Handle<v8::String> v8src = v8::String::NewFromUtf8(sdb->v8.iso, src);
  v8::Handle<v8::String> v8name = v8::String::NewFromUtf8(sdb->v8.iso, name);
  v8::TryCatch tc;

  // compile
  v8::ScriptOrigin origin(v8name);
  v8::Handle<v8::Script> script = v8::Script::Compile(v8src, &origin);
  if (script.IsEmpty()) {
    stordb_report_v8_exception(sdb, &tc);
    return 1;
  }

  // run
  v8::Handle<v8::Value> result = script->Run();

  // handle exceptions
  if (result.IsEmpty()) {
    if (!tc.HasCaught()) { return 1; }
    stordb_report_v8_exception(sdb, &tc);
  } else {
    if (tc.HasCaught()) { return 1; }
    if (!result->IsUndefined()) {
    }
  }
  return 0;
}
