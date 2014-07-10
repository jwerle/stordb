
#include <v8.h>
#include "stordb.h"
#include "exception.h"
#include "js.h"

v8::Handle<v8::Value>
stordb_runjs (stordb_t *sdb, const char *name, const char *src) {
  v8::EscapableHandleScope handle_scope(sdb->v8.iso);

  // script
  v8::Handle<v8::String> v8src = v8::String::NewFromUtf8(sdb->v8.iso, src);
  v8::Handle<v8::String> v8name = v8::String::NewFromUtf8(sdb->v8.iso, name);
  v8::TryCatch tc;

  // compile
  v8::ScriptOrigin origin(v8name);
  v8::Handle<v8::Script> script = v8::Script::Compile(v8src, &origin);
  if (script.IsEmpty()) {
    stordb_report_v8_exception(sdb, &tc);
    return v8::Null(sdb->v8.iso);
  }

  // run
  v8::Local<v8::Value> result = script->Run();

  // handle exceptions
  if (result.IsEmpty()) {
    if (!tc.HasCaught()) { return v8::Null(sdb->v8.iso); }
    stordb_report_v8_exception(sdb, &tc);
  } else {
    if (tc.HasCaught()) { return v8::Null(sdb->v8.iso); }
    if (!result->IsUndefined()) {
      printf("Fo\n");
    }
  }

  return handle_scope.Escape(result);
}

