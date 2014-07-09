
#ifndef STORDB_BINDINGS_H
#define STORDB_BINDINGS_H

#include <v8.h>
#include "stordb.h"

/**
 * stdio bindings
 */

void
stordb_bfprintf (const v8::FunctionCallbackInfo<v8::Value> &arguments);

void
stordb_bprintf (const v8::FunctionCallbackInfo<v8::Value> &arguments);

void
stordb_bprint (const v8::FunctionCallbackInfo<v8::Value> &arguments);

void
stordb_bprintf (const v8::FunctionCallbackInfo<v8::Value> &arguments);

v8::Handle<v8::Value>
stordb_bfread (const v8::FunctionCallbackInfo<v8::Value> &arguments);

v8::Handle<v8::Value>
stordb_bwrite (const v8::FunctionCallbackInfo<v8::Value> &arguments);

#endif
