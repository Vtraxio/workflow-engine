#include "http_common.h"
#include "net/connection.h"
#include "status.h"

typedef struct http_response {
  const char* version;
  http_status status;

  http_header* headers;

  u8* body;
  u32 body_length;
  u32 body_capacity;

  u8* internal;
  u32 internal_length;
  u32 internal_capacity;

  //   bool headers_sent;
} http_response;

bool http_response_create(http_response* out_response);
void http_response_destroy(http_response* response);

void http_response_set_status(http_response* response, http_status status);

bool http_response_set_header(http_response* response, const char* header,
                              const char* value);
bool http_response_set_content_type(http_response* response, const char* mime);
bool http_response_set_content_length(http_response* response, u32 length);

bool http_response_write(http_response* response, const void* data, u32 length);
bool http_response_write_str(http_response* response, const char* text);
__attribute__((format(printf, 2, 3))) bool
http_response_printf(http_response* response, const char* fmt, ...);

bool http_response_send(http_response* response, connection* con);

bool http_response_send_text(connection* con, http_status status,
                             const char* text);
bool http_response_send_html(connection* con, http_status status,
                             const char* html);
bool http_response_send_404(connection* con);