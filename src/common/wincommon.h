#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0A00

#include <sdkddkver.h>

#include <WS2tcpip.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <winsock2.h>

#include <common.h>

static inline wchar_t* win32_utf8_to_utf16(const char* str) {
  int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
  wchar_t* buffer = (wchar_t*)malloc(len * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, len);
  return buffer;
}

static inline char* win32_utf16_to_utf8(const wchar_t* str) {
  int len =
      WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
  char* buffer = (char*)malloc(len);
  WideCharToMultiByte(CP_UTF8, 0, str, -1, buffer, len, nullptr, nullptr);
  return buffer;
}

static inline void win32_fatal_error(const char* msg) {
  DWORD err = GetLastError();
  char buf[512];

  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 nullptr, err, 0, buf, sizeof(buf), nullptr);

  LOG_ERROR("%s: %s", msg, buf);
  DEBUG_BREAK();
}

static inline void* win32_alloc(size_t size) {
  void* ptr =
      VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  ASSERT(ptr);
  return ptr;
}

static inline void win32_free(void* ptr) {
  if (ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
  }
}

static inline HANDLE win32_open_file_read(const char* path) {
  wchar_t* wide_path = win32_utf8_to_utf16(path);

  HANDLE handle = CreateFileW(wide_path, GENERIC_READ, FILE_SHARE_READ, nullptr,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  free(wide_path);

  return handle;
}

void win32_print_wsa_error();

#define LOG_WSA_ERROR(...)                                                     \
  do {                                                                         \
    printf("[ERROR] ");                                                        \
    printf(__VA_ARGS__);                                                       \
    win32_print_wsa_error();                                                   \
    printf("\n");                                                              \
  } while (0)