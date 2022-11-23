#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "util.h"

uint32_t ipStringToNumber (const char* pStrIp, uint32_t* pIntIp)
{
    uint32_t    byte3;
    uint32_t    byte2;
    uint32_t    byte1;
    uint32_t    byte0;
    char        dummyString[2];

    if (sscanf (pStrIp, "%u.%u.%u.%u%1s", &byte3, &byte2, &byte1, &byte0, dummyString) == 4)
    {
        if ((byte3 < 256) && (byte2 < 256) && (byte1 < 256) && (byte0 < 256))
        {
            *pIntIp = (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
            return 1;
        }
    }
    return 0;
}
