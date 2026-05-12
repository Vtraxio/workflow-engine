#include "status.h"

const char* http_status_reason(http_status status) {
  switch (status) {
  case HTTP_STATUS_CONTINUE:
    return "Continue";
  case HTTP_STATUS_SWITCHING_PROTOCOLS:
    return "Switching Protocols";
  case HTTP_STATUS_OK:
    return "OK";
  case HTTP_STATUS_CREATED:
    return "Created";
  case HTTP_STATUS_ACCEPTED:
    return "Accepted";
  case HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION:
    return "Non-Authoritative Information";
  case HTTP_STATUS_NO_CONTENT:
    return "No Content";
  case HTTP_STATUS_RESET_CONTENT:
    return "Reset Content";
  case HTTP_STATUS_PARTIAL_CONTENT:
    return "Partial Content";
  case HTTP_STATUS_MULTIPLE_CHOICES:
    return "Multiple Choices";
  case HTTP_STATUS_MOVED_PERMANENTLY:
    return "Moved Permanently";
  case HTTP_STATUS_FOUND:
    return "Found";
  case HTTP_STATUS_SEE_OTHER:
    return "See Other";
  case HTTP_STATUS_NOT_MODIFIED:
    return "Not Modified";
  case HTTP_STATUS_USE_PROXY:
    return "Use Proxy";
  case HTTP_STATUS_TEMPORARY_REDIRECT:
    return "Temporary Redirect";
  case HTTP_STATUS_BAD_REQUEST:
    return "Bad Request";
  case HTTP_STATUS_UNAUTHORIZED:
    return "Unauthorized";
  case HTTP_STATUS_FORBIDDEN:
    return "Forbidden";
  case HTTP_STATUS_NOT_FOUND:
    return "Not Found";
  case HTTP_STATUS_METHOD_NOT_ALLOWED:
    return "Method Not Allowed";
  case HTTP_STATUS_NOT_ACCEPTABLE:
    return "Not Acceptable";
  case HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED:
    return "Proxy Authentication Required";
  case HTTP_STATUS_REQUEST_TIMEOUT:
    return "Request Timeout";
  case HTTP_STATUS_CONFLICT:
    return "Conflict";
  case HTTP_STATUS_GONE:
    return "Gone";
  case HTTP_STATUS_LENGTH_REQUIRED:
    return "Length Required";
  case HTTP_STATUS_PRECONDITION_FAILED:
    return "Precondition Failed";
  case HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE:
    return "Request Entity Too Large";
  case HTTP_STATUS_REQUEST_URI_TOO_LONG:
    return "Request-URI Too Long";
  case HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE:
    return "Unsupported Media Type";
  case HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE:
    return "Requested Range Not Satisfiable";
  case HTTP_STATUS_EXPECTATION_FAILED:
    return "Expectation Failed";
  case HTTP_STATUS_INTERNAL_SERVER_ERROR:
    return "Internal Server Error";
  case HTTP_STATUS_NOT_IMPLEMENTED:
    return "Not Implemented";
  case HTTP_STATUS_BAD_GATEWAY:
    return "Bad Gateway";
  case HTTP_STATUS_SERVICE_UNAVAILABLE:
    return "Service Unavailable";
  case HTTP_STATUS_GATEWAY_TIMEOUT:
    return "Gateway Timeout";
  case HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED:
    return "HTTP Version Not Supported";
  }
}