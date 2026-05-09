#include "utils.h"

void utils_dump_bytes(const void* data, u32 len) {
  (void)fwrite(data, 1, len, stdout);
}
