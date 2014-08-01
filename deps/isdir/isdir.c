
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "isdir.h"

int
isdir (const char *path) {
  struct stat st;
  if (NULL == path) { return 1; }
  stat(path, &st);
  return S_ISDIR(st.st_mode) ? 0 : 1;
}
