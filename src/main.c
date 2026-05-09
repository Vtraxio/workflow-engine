#include "net/connection.h"
#include "net/listener.h"
#include "utils/utils.h"
#include <stdio.h>

int main() {
  listener_create(8080);

  while (true) {
    connection socket;
    listener_listen(&socket);

    u8* buf = nullptr;
    u32 len = 0;
    connection_read_until(&socket, "\r\n\r\n", &buf, &len);
    utils_dump_bytes(buf, len);

    connection_printf(
        &socket, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 13\r\n\r\nHello, World!");
    connection_destroy(&socket);
    printf("\n\n\n");
  }

  return 0;
}
