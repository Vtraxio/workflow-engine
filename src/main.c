#include "net/connection.h"
#include "net/listener.h"
#include "utils/utils.h"
#include <stdio.h>

#include "net/http/request.h"
#include "net/http/response.h"

int main() {
  listener_create(8080);

  while (true) {
    connection socket;
    listener_listen(&socket);

    http_request req;
    http_request_create(&socket, &req);

    // u8* buf = nullptr;
    // u32 len = 0;
    // connection_read_until(&socket, "\r\n\r\n", &buf, &len);
    // utils_dump_bytes(buf, len);

    http_response_send_text(&socket, HTTP_STATUS_OK, "Hello, World!");

    // connection_printf(&socket,
    //                   "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: "
    //                   "13\r\n\r\nHello, World!");
    http_request_destroy(&req);
    connection_destroy(&socket);
    // printf("\n\n\n");
  }

  return 0;
}
