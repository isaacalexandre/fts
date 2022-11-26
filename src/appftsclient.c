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
#define PRINT_APP_FTS(args) do {printf ("\r\n[APP CLIENT FTS]: ");printf args;} while(0==1)

/******************************************************
 *                    Constants
 ******************************************************/
#define PORT_DEFAULT    (2323)
#define ADDR_DEFAULT    (0) //127.0.0.1
#define PATH_FILE_SIZE  (1000)

void usage() {
    printf("usage:\n");
    printf("./appftsClient [options]\n");
    printf("-d      debug enabled [0-1]\n");
    printf("        Default = 0\n\n");
    printf("-a      Address to connect (Ex.: 192.168.0.1)\n");
    printf("        Default = Local \n\n");
    printf("-p      Port to connect [1 - 65535]\n");
    printf("        Default = 2323\n\n");
    printf("-l      File path to send\n");
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
    int32_t c                       = 0;
    bool b_debug                    = 0;
    uint16_t port_fts               = PORT_DEFAULT; //Select port by default;
    uint32_t addr_fts               = ADDR_DEFAULT; //Select address by default;
    char path_file[PATH_FILE_SIZE]  = {'\0'};
    fts_result_t ret                = FTS_ERROR;

    while ((c = getopt (argc, argv, "d:p:a:l:h:stv")) != -1) {
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
        case 'l':
            strcpy(path_file, optarg);
            PRINT_APP_FTS(("File path: %s",path_file));
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
        client_fts_debug_enable(b_debug);

    PRINT_APP_FTS(("Start Server FTS"));

    //Create server socket
    PRINT_APP_FTS(("Start Socket create"));
    ret = client_fts_socket_init(port_fts, addr_fts);
    if( ret != FTS_SUCCESS ){
        PRINT_APP_FTS(("Fail to create socket: ret=[%d]",ret));
        goto out;
    }

    //Call to send file
    PRINT_APP_FTS(("Start send file process"));
    ret = client_fts_process_send_file((const char*)path_file);
    if( ret != FTS_SUCCESS ){
        PRINT_APP_FTS(("Fail send file: ret=[%d]",ret));
    }

out:
    //Destroy server socket
    ret = client_fts_socket_deinit();
    if( ret != FTS_SUCCESS ){
        PRINT_APP_FTS(("Fail to destroy socket: ret=[%d]",ret));
    }

    PRINT_APP_FTS((" "));

    return 0;
}
