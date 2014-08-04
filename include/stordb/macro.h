
#ifndef STORDB_MACRO_H
#define STORDB_MACRO_H

#include <v8.h>

#define SDBV8H(f, v) f(v8::Isolate::GetCurrent(), v)
#define V8STRING(v) SDBV8H(v8::String::NewFromUtf8, v)
#define V8FUNCTION(v) SDBV8H(v8::FunctionTemplate::New, v)
#define V8NUMBER(v) SDBV8H(v8::Number::New, v)
#define V8NULL() v8::Null(v8::Isolate::GetCurrent())
#define V8TRUE() v8::True(v8::Isolate::GetCurrent())
#define V8FALSE() v8::False(v8::Isolate::GetCurrent())
#define V8RETURN(value) ({ return args.GetReturnValue().Set(value); })
#define V8THROW(err) v8::Isolate::GetCurrent()->ThrowException(V8STRING(err))

#define V8OBJECT()                                                             \
  v8::ObjectTemplate::New(v8::Isolate::GetCurrent())->NewInstance()

#define STORDB_SET_BINDING(key, value) do {                                    \
  stordb_get_current()->v8.global->Set(V8STRING(key), value);                  \
} while (0);

#define V8SCOPE(args) ({                                                       \
  v8::Isolate *iso = args.GetIsolate();                                        \
  v8::Isolate::Scope isolate_scope(iso);                                       \
  v8::HandleScope scope(iso);                                                  \
})

#define V8DATE(t) SDBV8H(v8::Date::New, t)

#define V8WRAP(ptr) ({                                                         \
  v8::Handle<v8::ObjectTemplate> tpl =                                         \
    v8::ObjectTemplate::New(v8::Isolate::GetCurrent());                        \
  tpl->SetInternalFieldCount(1);                                               \
  v8::Local<v8::Object> obj = tpl->NewInstance();                              \
  obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), ptr)); \
  obj;                                                                         \
})

#define V8UNWRAP(ref) ({                                                       \
  v8::Local<v8::External> wrap =                                               \
    v8::Local<v8::External>::Cast(ref->GetInternalField(0));                   \
  wrap->Value();                                                               \
})

#endif
