/** @file
 *  Server File Transfer Socket Application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
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
#define PORT_DEFAULT    (2323)
#define ADDR_DEFAULT    (0) //127.0.0.1
#define PATH_FILE_SIZE  (1000)

void usage() {
    printf("usage:\n");
    printf("./appftsServer [options]\n");
    printf("-d      debug enabled [0-1]\n");
    printf("        Default = 0\n\n");
    printf("-a      Address Host Server (Ex.: 192.168.0.1)\n");
    printf("        Default = Local Host\n\n");
    printf("-p      Port Host Server [1 - 65535]\n");
    printf("        Default = 2323\n\n");
    printf("-l      Path to save file will receive\n");
    printf("        Default = Local where the program was executed\n\n");
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
    DIR* dir                        = NULL;
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
            if(ip_string_to_number(optarg, &addr_fts))
                PRINT_APP_FTS(("Address: %s",optarg));
            else
                PRINT_APP_FTS(("Invalid Address, IP used: 127.0.0.1"));            
            break;
        case 'l':
            strcpy(path_file, optarg);
            PRINT_APP_FTS(("Path: %s",path_file));
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

    //Verify if directory exist
    if(strlen(path_file) > 0) {
        PRINT_APP_FTS(("Verify if directory exist"));
        dir = opendir(path_file);
        if (dir) {
            closedir(dir);
            PRINT_APP_FTS(("Directory exist"));
        } else {
            PRINT_APP_FTS(("Directory not exist"));
            goto out;
        }
    }

    //Create server socket
    PRINT_APP_FTS(("Start Socket create"));
    ret = server_fts_socket_init(port_fts, addr_fts);
    if( ret != FTS_SUCCESS ){       
        PRINT_APP_FTS(("Fail to create socket: ret=[%d]",ret));
        goto out;
    }
    //Call to receive file
    PRINT_APP_FTS(("Start receive file process"));
    ret = server_fts_process_receive_file((const char*)path_file);
    if( ret != FTS_SUCCESS ){
        PRINT_APP_FTS(("Fail receiving file: ret=[%d]",ret));
    }
    
out:
    //Destroy server socket
    ret = server_fts_socket_deinit();
    if( ret != FTS_SUCCESS ){
        PRINT_APP_FTS(("Fail to destroy socket: ret=[%d]",ret));
    }
    
    return 0;
}
