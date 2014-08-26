
#include <v8.h>
#include "stordb.h"
#include "stordb/exception.h"
#include "stordb/js.h"

extern "C" {
#include <asprintf/asprintf.h>
}

v8::Handle<v8::Value>
stordb_runjs (stordb_t *sdb, const char *name, const char *src) {

#define RETURN(v) return handle_scope.Escape(v);

  //isolate
  v8::Isolate *iso = v8::Isolate::GetCurrent();
  // lock
  v8::Locker locker(iso);

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::EscapableHandleScope handle_scope(iso);

  // null value
  v8::Handle<v8::Value> null = v8::Null(iso);

  // script
  v8::Handle<v8::String> v8src = v8::String::NewFromUtf8(iso, src);
  v8::Handle<v8::String> v8name = v8::String::NewFromUtf8(iso, name);
  v8::TryCatch tc;

  // compile
  v8::ScriptOrigin origin(v8name);
  v8::Handle<v8::Script> script = v8::Script::Compile(v8src, &origin);
  if (script.IsEmpty()) {
    stordb_report_v8_exception(sdb, &tc);
    return null;
  }

  // run
  v8::Local<v8::Value> result = script->Run();

  // handle exceptions
  if (result.IsEmpty()) {
    if (tc.HasCaught()) {
      stordb_report_v8_exception(sdb, &tc);
    }
    return null;
  } else if (tc.HasCaught()) {
    return null;
  }

  RETURN(result);

#undef RETURN
}

