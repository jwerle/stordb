
#include <stdlib.h>
#include <v8.h>
#include "stordb.h"
#include "coro.h"

extern "C" {
#include <coro/coro.h>
}

typedef struct coro_wrap {
  coro_context *ctx;
  struct coro_stack stack;
  v8::Handle<v8::Function> fn;
} coro_wrap;

static coro_context root_ctx;

STORDB_MODULE(coro, {
  coro_create(&root_ctx, NULL, NULL, NULL, 0);
  STORDB_MODULE_SET(coro, "create", V8FUNCTION(stordb_coro_create));
  STORDB_MODULE_SET(coro, "enter", V8FUNCTION(stordb_coro_enter));
  STORDB_MODULE_SET(coro, "destroy", V8FUNCTION(stordb_coro_destroy));
});

static void
coro_call (void *data) {
  // isolate
  v8::Isolate *iso = v8::Isolate::GetCurrent();
  if (NULL == iso) {
    iso = v8::Isolate::New();
  }

  v8::Locker locker(iso);
  printf("lock\n");

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // scope
  v8::Isolate::Scope isolate_scope(iso);
  v8::EscapableHandleScope scope(iso);
  //v8::HandleScope scope(iso);

  // global
  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(iso);

  // context
  v8::Handle<v8::Context> ctx = v8::Context::New(iso, NULL, global);
  v8::Context::Scope context_scope(ctx);

  // coro wrap
  coro_wrap *wrap = (coro_wrap *) data;

  //ctx->Enter();
  // v8 routine
  printf("pre\n");
  wrap->fn->Call(global->NewInstance(), 0, NULL);
  printf("post\n");
  //ctx->Exit();
}

void
stordb_coro_create (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // coro struct wrap
  coro_wrap *wrap = NULL;

  if (1 != args.Length() || !args[0]->ToObject()->IsCallable()) {
    V8THROW("coro.create expects a function");
    return;
  }

  // coro v8 wrap
  v8::Handle<v8::ObjectTemplate> co = v8::ObjectTemplate::New(iso);

  // coro ctx
  coro_context *ctx = NULL;

  // alloc
  ctx = (struct coro_context *) malloc(sizeof(struct coro_context *));
  wrap = (struct coro_wrap *) malloc(sizeof(struct coro_wrap *));

  // create context
  coro_create(ctx, coro_call, wrap, wrap->stack.sptr, wrap->stack.ssze);

  // alloc stack
  coro_stack_alloc(&wrap->stack, 1);

  // init
  wrap->ctx = ctx;

  // attach callback
  wrap->fn = v8::Handle<v8::Function>::Cast(args[0]->ToObject());

  // set field count
  co->SetInternalFieldCount(1);

  // instance
  v8::Handle<v8::Object> instance = co->NewInstance();

  // set internal wrap
  instance->SetInternalField(0, V8EXTERNAL(wrap));

  V8RETURN(instance);
}

void
stordb_coro_enter (const v8::FunctionCallbackInfo<v8::Value> &args) {
  V8SCOPE(args);

  // isolate
  v8::Isolate *iso = args.GetIsolate();

  // coro struct wrap
  coro_wrap *wrap = NULL;

  if (1 != args.Length() || !args[0]->IsObject()) {
    V8THROW("coro.enter expects an object");
    return;
  }

  // unwrap
  v8::Local<v8::Object> co = args[0]->ToObject();
  v8::Local<v8::External> ref = v8::Local<v8::External>::Cast(co->GetInternalField(0));
  wrap = (coro_wrap *) ref->Value();

  coro_transfer(&root_ctx, wrap->ctx);
}

void
stordb_coro_destroy (const v8::FunctionCallbackInfo<v8::Value> &args) {
}

