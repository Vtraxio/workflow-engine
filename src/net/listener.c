#include "listener.h"

#include <wincommon.h>

struct internal_state {
  WSADATA wsa_data;
  bool wsa_initialized;
  SOCKET listen_socket;
};

static struct internal_state state;

bool listener_create(u16 port) {
  i32 error = 0;

  error = WSAStartup(MAKEWORD(2, 2), &state.wsa_data);
  if (error != 0) {
    LOG_ERROR("Failed to startup winsock: %d", error);
    return false;
  }
  state.wsa_initialized = true;

  struct addrinfoW* result = nullptr;
  struct addrinfoW hints = {0};

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Get a good address that we can use or whatever idk
  wchar_t port_str[6];
  _itow_s(port, port_str, ARRAY_COUNT(port_str), 10);

  error = GetAddrInfo(nullptr, port_str, &hints, &result);
  if (error != 0) {
    LOG_ERROR("Failed to startup winsock: %d", error);
    return false;
  }

  // Make the socket
  state.listen_socket =
      socket(result->ai_family, result->ai_socktype, result->ai_protocol);

  if (state.listen_socket == INVALID_SOCKET) {
    LOG_WSA_ERROR("Failed to create listen socket.");
    FreeAddrInfo(result);
    return false;
  }

  // Bind the socket
  error = bind(state.listen_socket, result->ai_addr, (int)result->ai_addrlen);
  if (error == SOCKET_ERROR) {
    LOG_WSA_ERROR("Failed to bind socket.");
    FreeAddrInfo(result);
    return false;
  }

  FreeAddrInfo(result);

  LOG_INFO("A listener has been created and bound to port: %d", port);

  return true;
}

void listener_destroy() {
  if (state.listen_socket) {
    closesocket(state.listen_socket);
  }
  if (state.wsa_initialized) {
    WSACleanup();
  }
}

bool listener_listen(connection* out_socket) {
  if (listen(state.listen_socket, SOMAXCONN) == SOCKET_ERROR) {
    LOG_WSA_ERROR("Failed to listen.");
    return false;
  }

  struct sockaddr_in addr;
  i32 size = sizeof(addr);

  SOCKET client_socket =
      accept(state.listen_socket, (struct sockaddr*)&addr, &size);
  if (client_socket == INVALID_SOCKET) {
    LOG_WSA_ERROR("Failed to accept.");
    return false;
  }

  LOG_INFO("Accepted socket: %d.%d.%d.%d:%d", addr.sin_addr.S_un.S_un_b.s_b1,
           addr.sin_addr.S_un.S_un_b.s_b2, addr.sin_addr.S_un.S_un_b.s_b3,
           addr.sin_addr.S_un.S_un_b.s_b4, addr.sin_addr.S_un.S_un_b.s_b1);

  i32 timeout = 1000;
  if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout,
                 sizeof(timeout)) == SOCKET_ERROR) {
    LOG_WSA_ERROR("Failed to set socket timeout parameter.");
    return false;
  }

  connection socket;
  socket.platform_socket = (void*)client_socket;
  socket.read_buf_cap = 4096;
  socket.read_buf_len = 0;
  socket.read_buf = malloc(socket.read_buf_cap);

  *out_socket = socket;
  return true;
}