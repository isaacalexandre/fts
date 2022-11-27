#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t ip_string_to_number (const char* pStrIp, uint32_t* pIntIp);

void printf_buffer_hex_char(const char* p_header, uint8_t* p_buffer, uint32_t size);

void get_filename_from_path(char* path, char ** filename);

bool file_crc32(uint32_t* crc, const char* path);

bool is_slash_end_str(const char* p_str);

#ifdef __cplusplus
}
#endif

#endif
