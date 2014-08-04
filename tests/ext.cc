
#include <v8.h>
#include <stordb.h>

STORDB_MODULE(ext, {
  STORDB_MODULE_SET(ext, "test", Test);
});

void Test (v8::FunctionCallbackInfo &args) {

}

