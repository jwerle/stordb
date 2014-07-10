
#ifndef STORDB_JS_H
#define STORDB_JS_H

#include <v8.h>
#include "stordb.h"

v8::Handle<v8::Value>
stordb_runjs (stordb_t *, const char *, const char *);

#endif
