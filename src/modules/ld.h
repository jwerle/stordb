
#ifndef STORB_LD_H
#define STORB_LD_H

#include "stordb.h"
#include "stordb/module.h"

STORDB_MODULE_DECLARE(ld);

void
stordb_ld_bopen (const v8::FunctionCallbackInfo<v8::Value> &);

#endif
