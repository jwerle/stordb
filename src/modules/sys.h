
#ifndef STORDB_SYS_H
#define STORDB_SYS_H

#include <v8.h>
#include <string.h>

#include "stordb/macro.h"

extern "C" {
#include <asprintf/asprintf.h>
}


void
stordb_sys_print (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_sys_load (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_sys_exit (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_sys_cwd (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_sys_chdir (const v8::FunctionCallbackInfo<v8::Value> &);

#endif
