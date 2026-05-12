#include <common.h>

typedef struct http_header {
  const char* key;
  const char* value;
} http_header;

bool http_headers_create(http_header** out_headers);
void http_headers_destroy(http_header* headers);

bool http_headers_set_header(http_header** headers, const char* header, const char* value);
bool http_headers_get_header(http_header* headers, const char* header,
                             const char** out_value);