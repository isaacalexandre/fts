/** @file
 *  Server File Transfer Socket Application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "util.h"
#include "libftsserver.h"

/******************************************************
 *                      Macros
 ******************************************************/
#define UNUSED(x) (void)(x)
#define PRINT_APP_FTS(args) do {printf ("\r\n[APP SERVER FTS]: ");printf args;} while(0==1)

/******************************************************
 *                    Constants
 ******************************************************/
#define PORT_DEFAULT (2323)
#define ADDR_DEFAULT ((uint32_t)0x7F000001) //127.0.0.1

void usage() {
    printf("usage:\n");
    printf("./appftsServer [options]\n");
    printf("-d      debug enabled [0-1]\n");
    printf("        Default = 0\n\n");
    printf("-a      Address Host Server (Ex.: 192.168.0.1)\n");
    printf("        Default = Local Host\n\n");
    printf("-p      Port Host Server [1 - 65535]\n");
    printf("        Default = 2323\n\n");
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
    uint16_t port_fts   = PORT_DEFAULT; //Select port by default;
    uint32_t addr_fts   = ADDR_DEFAULT; //Select address by default;
    fts_result_t ret    = FTS_ERROR;
 
    while ((c = getopt (argc, argv, "d:p:a:h:stv")) != -1) {
        switch (c) {
        case 'd':
            b_debug = (bool) atoi(optarg);
            PRINT_APP_FTS(("Debug enabled"));
            break;
        case 'p':
            port_fts = atoi(optarg);
            PRINT_APP_FTS(("Port number: %d",port_fts));
            break;
        case 'a':
            if(ipStringToNumber(optarg, &addr_fts))
                PRINT_APP_FTS(("Address: %s",optarg));
            else
                PRINT_APP_FTS(("Invalid Address, IP used: 127.0.0.1"));            
            break;
        case 'h':
        case '?':
        default:
            usage();
            break;
        }
    }

    //Verify if debug is enabled
    if(b_debug)
        server_fts_debug_enable(b_debug);

    PRINT_APP_FTS(("Start Server FTS"));

    //Create server socket
    ret = server_fts_socket_create(port_fts, addr_fts);
    if( ret != FTS_SUCCESS ){
        PRINT_APP_FTS(("Fail to create socket: ret=[%d]",ret));
    }

    return 0;
}
