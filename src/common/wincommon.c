#include "wincommon.h"

void win32_print_wsa_error() {
  DWORD error = WSAGetLastError();
  char buf[512];
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 nullptr, error, 0, buf, sizeof(buf), nullptr);

  for (u32 i = 0; i < strlen(buf); i++) {
    if (buf[i] == '\n' || buf[i] == '\r') {
      buf[i] = 0;
    }
  }

  printf(" (0x%lx: %s)", error, buf);
}
