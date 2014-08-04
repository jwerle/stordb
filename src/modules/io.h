
#ifndef STORDB_IO_H
#define STORDB_IO_H

#include <v8.h>
#include "stordb/module.h"

STORDB_MODULE_DECLARE(io);

void
stordb_io_read (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_io_write (const v8::FunctionCallbackInfo<v8::Value> &);

#endif
