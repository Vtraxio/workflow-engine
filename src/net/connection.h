#include <common.h>

typedef struct connection {
  void* platform_socket;

  u8* read_buf;
  u32 read_buf_cap;
  u32 read_buf_len;
} connection;

void connection_destroy(connection* connection);

bool connection_read_some(connection* con, u8* out, u32* inout_len);
bool connection_read_exact(connection* con, u8* out, u32 len);
bool connection_read_until(connection* con, const char* delimiter, u8** out_buf,
                           u32* out_len);
bool connection_peek(connection* con, u8** data, u32* len);
bool connection_write_all(connection* con, const void* data, u32 len);
__attribute__((format(printf, 2, 3))) bool
connection_printf(connection* con, const char* fmt, ...);