#include "response.h"

#include "utils/ds.h"
#include <stdarg.h>

bool http_response_create(http_response* out_response) {
  http_response result = {nullptr};
  if (!http_headers_create(&result.headers)) {
    return false;
  }

  result.version = "HTTP/1.1";
  http_response_set_status(&result, HTTP_STATUS_OK);
  result.body_capacity = 4096;
  result.body = malloc(result.body_capacity);
  result.internal_capacity = 64;
  result.internal = malloc(result.internal_capacity);

  memcpy(out_response, &result, sizeof(http_response));

  return true;
}

void http_response_destroy(http_response* response) {
  if (response->headers) {
    http_headers_destroy(response->headers);
    response->headers = nullptr;
  }
  if (response->headers) {
    http_headers_destroy(response->headers);
    response->headers = nullptr;
  }
  if (response->body) {
    free(response->body);
    response->body = nullptr;
  }
  if (response->internal) {
    free(response->internal);
    response->internal = nullptr;
  }
  response->body_length = 0;
  response->body_capacity = 0;
  response->internal_length = 0;
  response->internal_capacity = 0;
  response->version = nullptr;
}

void http_response_set_status(http_response* response, http_status status) {
  response->status = status;
}

bool http_response_set_header(http_response* response, const char* header,
                              const char* value) {
  u32 length = strlen(value) + 1;
  if (response->internal_capacity < response->internal_length + length) {
    u32 new_cap = response->internal_capacity * 2;
    while (new_cap < response->internal_capacity + length) {
      new_cap *= 2;
    }

    u8* new_buf = realloc(response->internal, new_cap);
    if (!new_buf) {
      LOG_ERROR("Failed to resize http response internal header buffer.");
      return false;
    }
  }
  u8* pos = response->internal + response->internal_length;
  memcpy(pos, value, length);
  response->internal_length += length;

  return http_headers_set_header(&response->headers, header, (char*)pos);
}

bool http_response_set_content_type(http_response* response, const char* mime) {
  return http_headers_set_header(&response->headers, "content-type", mime);
}

bool http_response_set_content_length(http_response* response, u32 length) {
  char str[_MAX_ULTOSTR_BASE10_COUNT];
  _ultoa_s(length, str, _MAX_ULTOSTR_BASE10_COUNT, 10);
  http_response_set_header(response, "content-length", str);
  return true;
}

bool http_response_write(http_response* response, const void* data,
                         u32 length) {
  if (response->body_capacity < response->body_length + length) {
    u32 new_cap = response->body_capacity * 2;
    while (new_cap < response->body_capacity + length) {
      new_cap *= 2;
    }

    u8* new_buf = realloc(response->body, new_cap);
    if (!new_buf) {
      LOG_ERROR("Failed to resize http response body buffer.");
      return false;
    }
  }

  memcpy(response->body + response->body_length, data, length);
  response->body_length += length;
  return true;
}

bool http_response_write_str(http_response* response, const char* text) {
  u32 length = strlen(text);
  return http_response_write(response, text, length);
}

bool http_response_printf(http_response* response, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  va_list args_copy;
  va_copy(args_copy, args);
  i32 needed = vsnprintf(nullptr, 0, fmt, args_copy);
  va_end(args_copy);

  if (needed < 0) {
    va_end(args);
    return false;
  }

  char* buffer = malloc(needed + 1);
  (void)vsnprintf(buffer, needed + 1, fmt, args);
  va_end(args);

  bool res = http_response_write(response, buffer, (u32)needed);

  free(buffer);
  return res;
}

bool http_response_send(http_response* response, connection* con) {
  http_response_set_content_length(response, response->body_length);

  connection_printf(con, "%s %d %s\r\n", response->version, response->status,
                    http_status_reason(response->status));

  for (u32 i = 0; i < shlen(response->headers); i++) {
    http_header* header = &response->headers[i];
    connection_printf(con, "%s: %s\r\n", header->key, header->value);
  }
  connection_printf(con, "\r\n");
  if (response->body_length > 0) {
    connection_write_all(con, response->body, response->body_length);
  }

  return true;
}

bool http_response_send_text(connection* con, http_status status,
                             const char* text) {
  http_response res;
  http_response_create(&res);
  http_response_set_status(&res, status);
  http_response_set_content_type(&res, "text/plain");
  http_response_write_str(&res, text);
  http_response_send(&res, con);
  http_response_destroy(&res);
  return true;
}

bool http_response_send_html(connection* con, http_status status,
                             const char* html) {
  http_response res;
  http_response_create(&res);
  http_response_set_status(&res, status);
  http_response_set_content_type(&res, "text/html");
  http_response_write_str(&res, html);
  http_response_send(&res, con);
  http_response_destroy(&res);
  return true;
}

bool http_response_send_404(connection* con) {
  http_response res;
  http_response_create(&res);
  http_response_set_status(&res, HTTP_STATUS_NOT_FOUND);
  http_response_send(&res, con);
  http_response_destroy(&res);
  return true;
}