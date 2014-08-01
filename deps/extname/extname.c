
#include "extname.h"
#include <string.h>

const char *extname(const char *filename)
{
  const char *slash = strrchr(filename, '/');
  const char *loc = strrchr(slash? slash : filename, '.');
  return loc? loc : "";
}
