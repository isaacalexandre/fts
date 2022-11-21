/** @file
 *  Client File Transfer Socket Application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "util.h"
#include "libftsclient.h"

/******************************************************
 *                      Macros
 ******************************************************/
#define UNUSED(x) (void)(x)

/******************************************************
 *                    Constants
 ******************************************************/

void usage() {
    printf("usage:\n");
    printf("./appftsClient [options]\n");
    printf("-d      debug enabled [0-1]\n");
    printf("        Default = 0\n\n");
    printf("\r\n");
    exit(1);
}

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/
 
/******************************************************
 *               Function Definitions
 ******************************************************/

/******************************************************
 *               Interface functions
 ******************************************************/

int main (int argc, char *argv[])
{
    int32_t c           = 0;
    bool b_debug        = 0;
 
    while ((c = getopt (argc, argv, "d:h:stv")) != -1) {
        switch (c) {
        case 'd':
            b_debug = (bool) atoi(optarg);;
            break;
        case 'h':
        case '?':
        default:
            usage();
            break;
        }
    }
    UNUSED(b_debug);

    printf("\n\nClient FTS\n\n");

    return fts_test();
}
