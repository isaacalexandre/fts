#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <unistd.h>   

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "libftsserver.h"
#include "util.h"

/******************************************************
 *               Variable Definitions
 ******************************************************/
int sockfd_ = 0; //Socket File Descriptor
struct sockaddr_in serveraddr_; 
bool b_debug = false; 

/******************************************************
 *                      Macros
 ******************************************************/
#define UNUSED(x) (void)(x)
#define LISTENQ 8 /*maximum number of client connections */
#define DEBUG_FTS(args) do {if(b_debug){printf ("\r\n[SERVER FTS]: ");printf args;}} while(0==1)

/******************************************************
 *                    Constants
 ******************************************************/

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
 *               Function Definitions
 ******************************************************/

/******************************************************
 *               Interface functions
 ******************************************************/
void server_fts_debug_enable(bool enabled)
{
    b_debug = enabled;
}

fts_result_t server_fts_socket_create(uint16_t port, uint32_t ip_server)
{
    fts_result_t ret = FTS_ERROR;

    //Verify if already created
    if(sockfd_ != 0){
        DEBUG_FTS(("Socket already created"));
        ret = FTS_INVALID_SOCKET;
        goto out;  
    }

    //Create Socket
    DEBUG_FTS(("Create socket"));
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

    //Check if socket was created
    if(sockfd_ == -1){
        DEBUG_FTS(("Fail create socket"));
        ret = FTS_SOCKET_CREATE_FAIL;
        goto out;
    }

    DEBUG_FTS(("IP: [%u]",ip_server));

    //Set parameters
    memset(&serveraddr_, 0, sizeof(serveraddr_));
    serveraddr_.sin_family      = AF_INET;
    serveraddr_.sin_addr.s_addr = htonl(ip_server);
    serveraddr_.sin_port        = htons(port);

    //Specify the port
    DEBUG_FTS(("Specify the port: BIND"));
    if(bind(sockfd_, (struct sockaddr *) &serveraddr_, sizeof(serveraddr_)) < 0 ){
        DEBUG_FTS(("Fail bind port"));
        ret = FTS_BIND_ERROR;
        goto out; 
    }

    //Start listen the port
    DEBUG_FTS(("Start listen port"));
    if(listen(sockfd_, LISTENQ) < 0){
        DEBUG_FTS(("Fail listen port"));
        ret = FTS_LISTEN_ERROR;
        goto out;
    }

    ret = FTS_SUCCESS;
out:
    return ret;
}
