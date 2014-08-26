
#ifndef STORDB_CORO_H
#define STORDB_CORO_H

#include <v8.h>
#include "stordb/module.h"

STORDB_MODULE_DECLARE(coro);

void
stordb_coro_create (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_coro_enter (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_coro_destroy (const v8::FunctionCallbackInfo<v8::Value> &);

#endif
