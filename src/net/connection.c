#include "connection.h"

#include <wincommon.h>

static bool connection_reserve(connection* con, u32 extra);
static bool connection_fill_from_socket(connection* con);
static void connection_consume(connection* con, u32 amount);

void connection_destroy(connection* con) {
  SOCKET socket = (SOCKET)con->platform_socket;

  if (socket && socket != INVALID_SOCKET) {
    closesocket(socket);
    con->platform_socket = nullptr;
  }
  if (con->read_buf) {
    free(con->read_buf);
    con->read_buf = nullptr;
  }
  con->read_buf_cap = 0;
  con->read_buf_len = 0;
}

bool connection_read_some(connection* con, u8* out, u32* inout_len) {
  SOCKET socket = (SOCKET)con->platform_socket;
  u32 wanted = *inout_len;

  while (con->read_buf_len == 0) {
    if (!connection_fill_from_socket(con)) {
      return false;
    }
  }

  u32 give = con->read_buf_len;
  if (give > wanted) {
    give = wanted;
  }

  memcpy(out, con->read_buf, give);

  memmove(con->read_buf, con->read_buf + give, con->read_buf_len - give);

  con->read_buf_len -= give;

  *inout_len = give;
  return true;
}

bool connection_read_exact(connection* con, u8* out, u32 len) {
  SOCKET socket = (SOCKET)con->platform_socket;
  u32 wanted = len;

  while (con->read_buf_len < wanted) {
    if (!connection_fill_from_socket(con)) {
      return false;
    }
  }

  u32 give = con->read_buf_len;
  if (give > wanted) {
    give = wanted;
  }

  memcpy(out, con->read_buf, give);

  memmove(con->read_buf, con->read_buf + give, con->read_buf_len - give);

  con->read_buf_len -= give;
  return true;
}

bool connection_read_until(connection* con, const char* delimiter, u8** out_buf,
                           u32* out_len) {
  const u32 delim_len = strlen(delimiter);

  while (true) {
    u8* buf;
    u32 buf_len;
    if (!connection_peek(con, &buf, &buf_len)) {
      return false;
    }

    for (u32 i = 0; i + delim_len <= buf_len; i++) {
      if (memcmp(buf + i, delimiter, delim_len) == 0) {
        u32 chunk_len = i;

        u8* result = malloc(chunk_len);

        memcpy(result, buf, chunk_len);

        connection_consume(con, i + delim_len);

        *out_buf = result;
        *out_len = chunk_len;
        return true;
      }
    }

    connection_fill_from_socket(con);
  }

  return true;
}

bool connection_peek(connection* con, u8** data, u32* len) {
  if (con->read_buf_len == 0) {
    if (!connection_fill_from_socket(con)) {
      return false;
    }
  }

  *data = con->read_buf;
  *len = con->read_buf_len;
  return true;
}

bool connection_write_all(connection* con, const void* data, u32 len) {
  SOCKET socket = (SOCKET)con->platform_socket;

  i32 sent = send(socket, data, (i32)len, 0);
  if (sent == SOCKET_ERROR) {
    LOG_WSA_ERROR("Failed to send to socket.");
    return false;
  }

  return true;
}

bool connection_printf(connection* con, const char* fmt, ...) {
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

  bool res = connection_write_all(con, buffer, (u32)needed);

  free(buffer);
  return res;
}

static bool connection_reserve(connection* con, u32 extra) {
  if (con->read_buf_len + extra <= con->read_buf_cap) {
    return true;
  }

  u32 new_cap = con->read_buf_cap * 2;
  while (new_cap < con->read_buf_len + extra) {
    new_cap *= 2;
  }

  u8* new_buf = realloc(con->read_buf, new_cap);

  con->read_buf = new_buf;
  con->read_buf_cap = new_cap;
  return true;
}

static bool connection_fill_from_socket(connection* con) {
  SOCKET socket = (SOCKET)con->platform_socket;
  u8 tmp[4096];

  i32 received = recv(socket, (char*)tmp, sizeof(tmp), 0);
  if (received == SOCKET_ERROR) {
    LOG_WSA_ERROR("Failed to read socket.");
    return false;
  }

  if (!connection_reserve(con, received)) {
    return false;
  }

  memcpy(con->read_buf + con->read_buf_len, tmp, received);
  con->read_buf_len += received;
  return true;
}

static void connection_consume(connection* con, u32 amount) {
  if (amount >= con->read_buf_len) {
    con->read_buf_len = 0;
    return;
  }

  memmove(con->read_buf, con->read_buf + amount, con->read_buf_len - amount);

  con->read_buf_len -= amount;
}