
#ifndef STORDB_FS_H
#define STORDB_FS_H

#include <v8.h>

extern "C" {
#include <fs/fs.h>
}

#include "stordb.h"
#include "stordb/module.h"

STORDB_MODULE_DECLARE(fs);

void
stordb_fs_berror (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bopen (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bclose (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_brename (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bstat (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfstat (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_blstat (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bftruncate (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_btruncate (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bchown (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfchown (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_blchown (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bsize (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfsize (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bread (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bnread (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfread (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfnread (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bwrite (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfwrite (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bfnwrite (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bmkdir (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_brmdir (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bexists (const v8::FunctionCallbackInfo<v8::Value> &);

void
stordb_fs_bseek (const v8::FunctionCallbackInfo<v8::Value> &);

#endif
