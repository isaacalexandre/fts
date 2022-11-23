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
#define STEP_PRINT 100
static void server_fts_printf_buffer_(uint8_t* p_buffer, uint32_t size)
{
    if(b_debug && size > 0){
        uint32_t size_aux = size;
        uint32_t print_step = 0;
        uint8_t* p_buffer_char = p_buffer;
    
        printf("\r\n[SERVER FTS]: Frame log - %d bytes\r\n", size);
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

            for (size_t i = 0; i < print_step; i++) 
                printf("%X ", *(p_buffer++));
            
            printf("   ");

            for (size_t i = 0; i < print_step; i++) 
                printf("%c", *(p_buffer_char++));

            printf("\r\n");
        }
        printf("\r\n");
    }
}

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
    UNUSED(path);
    int loopfd;
    int rcv_data_len = 0;
    uint8_t buffer[BUFFER_SIZE] = {0};

    while (1) {
        DEBUG_FTS(("Call accept"));
        loopfd = accept(sockfd_, NULL, NULL);
        DEBUG_FTS(("Accepted - Socket FD: [%d]", sockfd_));
        DEBUG_FTS(("Call RECV"));
        rcv_data_len = recv(loopfd, buffer, BUFFER_SIZE, 0);
        DEBUG_FTS(("Received %d bytes", rcv_data_len));
        DEBUG_FTS(("Call SEND %d byte", rcv_data_len));
        send(loopfd, buffer, rcv_data_len , 0);

        //Print buffer
        server_fts_printf_buffer_(buffer, rcv_data_len);
        
        close(loopfd);
    }

    return FTS_SUCCESS;
}

fts_result_t server_fts_socket_deinit(void)
{
    return (close(sockfd_) < 0 ? FTS_ERROR : FTS_SUCCESS);
}
