
#ifndef STORDB_EXCEPTION_H

#include <v8.h>
#include "stordb.h"

/**
 * Prints a caught exception to `stderr'
 */

void
stordb_report_v8_exception (stordb_t *, v8::TryCatch *);

#endif
