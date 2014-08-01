
#ifndef STORDB_FS_H
#define STORDB_FS_H

#include <v8.h>

extern "C" {
#include <fs/fs.h>
}

#include "stordb.h"

void
stordb_fs_bread (const v8::FunctionCallbackInfo<v8::Value> &);

#endif
