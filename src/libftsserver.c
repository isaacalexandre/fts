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
#define LISTENQ 8 //Maximum number of client connections 
#define DEBUG_FTS(args) do {if(b_debug){printf ("\r\n[SERVER FTS]: ");printf args;}} while(0==1)
#define DEBUG_FTS_FRAME(buffer,size) do {if(b_debug){printf_buffer_hex_char("SERVER FTS",buffer,size);}} while(0==1)

/******************************************************
 *                    Constants
 ******************************************************/
#define BUFFER_SIZE (2048)

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

fts_result_t server_fts_socket_init(uint16_t port, uint32_t ip_server)
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
    DEBUG_FTS(("Finish socket create"));

    ret = FTS_SUCCESS;
out:
    return ret;
}

fts_result_t server_fts_process_receive_file(const char* path)
{
    int loopfd;
    int rcv_data_len = 0;
    uint8_t buffer[BUFFER_SIZE] = {0};
    fts_result_t ret = FTS_ERROR;

    //Sanity check
    if(sockfd_ <= 0) {
        DEBUG_FTS(("Socket not created"));
        ret = FTS_INVALID_SOCKET;
        goto out;
    }
    if(path == NULL){
        DEBUG_FTS(("Path pointer is null"));
        ret = FTS_BADARG;
        goto out;
    }

    //Accept request
    DEBUG_FTS(("Call accept"));
    loopfd = accept(sockfd_, NULL, NULL);
    DEBUG_FTS(("Accepted - Socket FD: [%d]", sockfd_));

    while (1) {
        
        DEBUG_FTS(("Call RECV"));
        rcv_data_len = recv(loopfd, buffer, BUFFER_SIZE, 0);
        DEBUG_FTS(("Received %d bytes", rcv_data_len));
        //Print buffer
        DEBUG_FTS_FRAME(buffer, rcv_data_len);

        DEBUG_FTS(("Call SEND %d byte", rcv_data_len));
        send(loopfd, "1", 1 , 0);
    }
    
    close(loopfd);

    ret = FTS_SUCCESS;
out:
    return ret;
}

fts_result_t server_fts_socket_deinit(void)
{
    DEBUG_FTS(("Destroy socket"));
    return (close(sockfd_) < 0 ? FTS_ERROR : FTS_SUCCESS);
}
