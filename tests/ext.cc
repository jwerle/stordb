
#include <v8.h>
#include <stordb.h>

static void
hello (const v8::FunctionCallbackInfo<v8::Value> &);

extern "C" void
Init (v8::Isolate *iso, v8::Handle<v8::ObjectTemplate> exports) {
  v8::Isolate::Scope isolate_scope(iso);
  v8::Handle<v8::ObjectTemplate> o = v8::ObjectTemplate::New(iso);
  exports->Set(V8STRING("hello"), V8FUNCTION(hello));
}

static void
hello (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);
  printf("hello world!\n");
}
