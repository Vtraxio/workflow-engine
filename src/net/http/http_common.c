#include "http_common.h"

#include "utils/ds.h"

bool http_headers_create(http_header** out_headers) {
  sh_new_arena(*out_headers);
  return true;
}

void http_headers_destroy(http_header* headers) { shfree(headers); }

bool http_headers_set_header(http_header* headers, const char* header, const char* value) {
  shput(headers, header, value);
  return true;
}

bool http_headers_get_header(http_header* headers, const char* header,
                             const char** out_value) {
  http_header* headerp = shgetp_null(headers, header);

  if (headerp == nullptr) {
    *out_value = nullptr;
    return false;
  }

  if (out_value) {
    *out_value = headerp->value;
  }

  return true;
}