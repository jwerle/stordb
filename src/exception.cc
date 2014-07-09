
#include <stdio.h>
#include <stdlib.h>
#include <v8.h>

#include "stordb.h"
#include "exception.h"

#define cstr(v) (*v? *v : "")

void
stordb_report_v8_exception (stordb_t *sdb, v8::TryCatch* tc) {
  v8::HandleScope handle_scope(sdb->v8.iso);
  v8::String::Utf8Value exception(tc->Exception());
  const char* exception_string = cstr(exception);
  v8::Handle<v8::Message> message = tc->Message();

  if (message.IsEmpty()) {
    fprintf(stderr, "%s\n", exception_string);
  } else {
    v8::String::Utf8Value filename(message->GetScriptResourceName());
    v8::String::Utf8Value sourceline(message->GetSourceLine());
    v8::String::Utf8Value stack_trace(tc->StackTrace());
    const char* filename_string = cstr(filename);
    int i = 0;
    int linenum = message->GetLineNumber();
    int start = message->GetStartColumn();
    int end = message->GetEndColumn();
    const char* sourceline_string = cstr(sourceline);
    const char* stack_trace_string = NULL;

    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    fprintf(stderr, "%s\n", sourceline_string);

    for (i = 0; i < start; i++) { fprintf(stderr, " "); }
    for (i = start; i < end; i++) { fprintf(stderr, "^"); }

    fprintf(stderr, "\n");

    if (stack_trace.length() > 0) {
      stack_trace_string = cstr(stack_trace);
      fprintf(stderr, "%s\n", stack_trace_string);
    }
  }
}

