#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "util.h"

#define STEP_PRINT 30
void printf_buffer_hex_char(const char* p_header, uint8_t* p_buffer, uint32_t size)
{
    uint32_t size_aux = size;
    uint32_t print_step = 0;
    uint8_t* p_buffer_char = p_buffer;
    char c_aux = '\0';

    //Verify if have bytes to print
    if(size > 0){
        printf("\r\n[%s]: Frame log - %d bytes\r\n", p_header, size);
        while(size_aux != 0){
            if(size_aux <= STEP_PRINT){
                print_step = size_aux;
                size_aux = 0;
            } else {
                print_step = STEP_PRINT;
                size_aux -= STEP_PRINT;
            }

            //Hold the pointer to print in char
            p_buffer_char = p_buffer;
            
            printf("   ");
            //Print Hex
            for (size_t i = 0; i < print_step; i++) 
                printf("%02X ", *(p_buffer++));

            //Normalize last line
            if(print_step < STEP_PRINT){
                for (size_t i = 0; i < (STEP_PRINT - print_step); i++)
                    printf("   ");
            }
            
            printf("   ");
            //Print char mode
            for (size_t i = 0; i < print_step; i++) {
                c_aux = *(p_buffer_char++);
                printf("%c", (c_aux > 0x00 && c_aux < 0x1F ? 0xFF : c_aux) );
            }

            printf("\r\n");
        }
        printf("\r\n");
    }
}

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
