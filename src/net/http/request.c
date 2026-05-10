#include "request.h"

#include "utils/utils.h"

bool http_request_create(connection* con, http_request* out_request) {
  http_request result = {nullptr};
  http_headers_create(&result.headers);

  u8* raw_buf = nullptr;
  u32 raw_len = 0;
  if (!connection_read_until(con, "\r\n\r\n", &raw_buf, &raw_len)) {
    return false;
  }

  result.internal_text = malloc(raw_len + 1);
  memcpy(result.internal_text, raw_buf, raw_len);
  result.internal_text[raw_len] = 0;
  free(raw_buf);

  // 1st line
  char* body_context;
  char* request_line = strtok_s(result.internal_text, "\r\n", &body_context);

  // request line parsing
  char* request_line_context;
  result.method = strtok_s(request_line, " ", &request_line_context);
  result.path = strtok_s(nullptr, " ", &request_line_context);
  result.version = strtok_s(nullptr, " ", &request_line_context);

  // header parsing
  char* header_line;
  while ((header_line = strtok_s(nullptr, "\r\n", &body_context)) != nullptr) {
    char* header_context;
    char* header_name = strtok_s(header_line, ":", &header_context);
    char* header_value = strtok_s(nullptr, ":", &header_context);

    if (!header_name || !header_value) {
      LOG_WARN("Could not parse header value.");
      continue;
    }

    string_lower(header_name);
    while (header_value[0] == ' ') {
      header_value++;
    }

    http_headers_set_header(result.headers, header_name, header_value);
  }

  // body
  const char* content_length_str;
  if (http_headers_get_header(result.headers, "content-length",
                              &content_length_str)) {
    u32 content_length = strtol(content_length_str, nullptr, 10);

    result.body = malloc(content_length);
    if (connection_read_exact(con, result.body, content_length)) {
      result.body_length = content_length;
    } else {
      free(result.body);
      result.body = nullptr;
    }
  }

  memcpy(out_request, &result, sizeof(http_request));

  return true;
}

void http_request_destroy(http_request* request) {
  if (request->internal_text) {
    free(request->internal_text);
    request->internal_text = nullptr;
  }
  if (request->headers) {
    http_headers_destroy(request->headers);
    request->headers = nullptr;
  }
  if (request->body) {
    free(request->body);
    request->body = nullptr;
  }
  request->method = nullptr;
  request->path = nullptr;
  request->version = nullptr;
  request->body_length = 0;
}