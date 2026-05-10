#include "utils.h"

#include <ctype.h>

void utils_dump_bytes(const void* data, u32 len) {
  (void)fwrite(data, 1, len, stdout);
}

void string_lower(char* str) {
  for (int i = 0; str[i]; i++) {
    str[i] = (char)tolower(str[i]);
  }
}