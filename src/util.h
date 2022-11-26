#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t ipStringToNumber (const char* pStrIp, uint32_t* pIntIp);

void printf_buffer_hex_char(const char* p_header, uint8_t* p_buffer, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
