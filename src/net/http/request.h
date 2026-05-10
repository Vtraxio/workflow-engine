#include "http_common.h"
#include "net/connection.h"

typedef struct http_request {
  char* internal_text;

  char* method;
  char* path;
  char* version;

  http_header* headers;

  u8* body;
  u32 body_length;
} http_request;

bool http_request_create(connection* con, http_request* out_request);
void http_request_destroy(http_request* request);