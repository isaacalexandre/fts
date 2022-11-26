#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <arpa/inet.h>
#include <unistd.h>   

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "libftsclient.h"
#include "util.h"

/******************************************************
 *               Variable Definitions
 ******************************************************/
int sockfd_ = 0; //Socket File Descriptor
struct sockaddr_in destaddr_; 
bool b_debug = false; 

/******************************************************
 *                      Macros
 ******************************************************/
#define UNUSED(x) (void)(x)
#define LISTENQ 8 /*maximum number of client connections */
#define DEBUG_FTS(args) do {if(b_debug){printf ("\r\n[CLIENT FTS]: ");printf args;}} while(0==1)
#define DEBUG_FTS_FRAME(buffer,size) do {if(b_debug){printf_buffer_hex_char("CLIENT FTS",buffer,size);}} while(0==1)

/******************************************************
 *                    Constants
 ******************************************************/
#define BUFFER_SIZE (512)
#define SEND_RETRIES (3)

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
static fts_result_t client_fts_send_file_(FILE* p_file, size_t size)
{
    fts_result_t ret = FTS_ERROR;
    uint8_t buffer[BUFFER_SIZE] = {0};
    size_t offset = 0;
    size_t size_buffer_send = 0;
    uint16_t err_count = 0;
    size_t bytes_read = 0;

    //Sanity
    if(p_file == NULL){
        DEBUG_FTS(("Pointer null"));
        ret = FTS_BADARG;
        goto out;
    }

    while(offset != size){
        err_count = 0;

        DEBUG_FTS(("Seek file to offset: [0x%lX]", (long unsigned int)offset));
        if(fseek(p_file, offset, SEEK_SET) == -1){
            DEBUG_FTS(("Error seek file"));
            ret = FTS_SEEK_FILE_ERROR;
            goto out;
        }

        //Verify the size will send
        if((size - offset) <= BUFFER_SIZE)
            size_buffer_send = (size - offset);
        else
            size_buffer_send = BUFFER_SIZE;

        //Read the file 
        DEBUG_FTS(("Read file with buffer size: [%lu]",(long unsigned int) size_buffer_send));
        if(size_buffer_send != fread(buffer, 1, size_buffer_send, p_file)){
            DEBUG_FTS(("Read file error"));
            ret = FTS_READ_FILE_ERROR;
            goto out;
        }

        //Send the chunk 
        DEBUG_FTS(("Send chunk to the server"));
        if(size_buffer_send != (size_t)send(sockfd_, buffer, size_buffer_send, 0)){
            //Verify if try again or abort
            if(++err_count > SEND_RETRIES){
                DEBUG_FTS(("Try again, error count = [%d]", err_count));
                continue;
            }
            DEBUG_FTS(("Error send chunk"));
            ret = FTS_SEND_PACKET_ERROR;
            goto out;
        }

        //Debug frame send
        DEBUG_FTS_FRAME(buffer, size_buffer_send);

        //Verify the response
        DEBUG_FTS(("Receive response"));
        bzero(buffer, BUFFER_SIZE);
        do{
            bytes_read = recv(sockfd_, buffer, BUFFER_SIZE, 0);
        }while(bytes_read <= 0);
        
        if(bytes_read <= 0){
            if(++err_count > SEND_RETRIES){
                DEBUG_FTS(("Error response try again, error count = [%d]", err_count));
                continue;
            }
            DEBUG_FTS(("Error response: abort"));
            ret = FTS_RECV_PACKET_ERROR;
            goto out;
        }

        //Set the offset
        offset += size_buffer_send;
    }
    ret = FTS_SUCCESS;
out:
    return ret;
}

static fts_result_t client_fts_open_file_(const char* path, FILE** p_file, size_t* p_size)
{
    fts_result_t ret = FTS_ERROR;

    //Sanity
    if(path == NULL || p_size == NULL){
        DEBUG_FTS(("Pointer null"));
        ret = FTS_BADARG;
        goto out;
    }

    //Open file
    DEBUG_FTS(("Open file at path: [%s]", path));
    *p_file = fopen(path,"rb");
    if (p_file == NULL){
        DEBUG_FTS(("Error opening file"));
        ret = FTS_OPEN_FILE_ERROR;
        goto out;
    }

    DEBUG_FTS(("Seek end of file"));
    if(fseek(*p_file, 0x00, SEEK_END) == -1){
        DEBUG_FTS(("Error seek file"));
        ret = FTS_SEEK_FILE_ERROR;
        goto out;
    }

    //Get the file size
    *p_size = ftell(*p_file);
    DEBUG_FTS(("Size of the file: [%lu]", (long unsigned int)*p_size));

    DEBUG_FTS(("Seek start of file"));
    if(fseek(*p_file, 0x00, SEEK_SET) == -1){
        DEBUG_FTS(("Error seek file"));
        ret = FTS_SEEK_FILE_ERROR;
        goto out;
    }

    ret = FTS_SUCCESS;
out:
    return ret;
}

void client_fts_debug_enable(bool enabled)
{
    b_debug = enabled;
}

fts_result_t client_fts_socket_init(uint16_t port, uint32_t ip_server)
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
    memset(&destaddr_, 0, sizeof(destaddr_));
    destaddr_.sin_family      = AF_INET;
    destaddr_.sin_addr.s_addr = htonl(ip_server);
    destaddr_.sin_port        = htons(port);

    //Try connect to the server
    DEBUG_FTS(("Try to connect the server"));
    if ( connect(sockfd_, (struct sockaddr *)&destaddr_, sizeof(destaddr_)) != 0 ) {
        DEBUG_FTS(("Fail to connect to server"));
        ret = FTS_BIND_ERROR;
        goto out; 
    }

    DEBUG_FTS(("Finish socket create"));

    ret = FTS_SUCCESS;
out:
    return ret;
}

fts_result_t client_fts_process_send_file(const char* path)
{
    static FILE *p_file = NULL;
    size_t file_size = 0;    
    fts_result_t ret = FTS_ERROR;
    
    //Sanity check
    if(sockfd_ <= 0){
        DEBUG_FTS(("Socket not created"));
        ret = FTS_INVALID_SOCKET;
        goto out;
    }
    if(path == NULL){
        DEBUG_FTS(("Path pointer is null"));
        ret = FTS_BADARG;
        goto out;
    }

    ret = client_fts_open_file_(path, &p_file, &file_size);
    if(ret != FTS_SUCCESS){
        DEBUG_FTS(("Fail open file"));
        goto out;
    }

    DEBUG_FTS(("Send file"));
    ret = client_fts_send_file_(p_file, file_size);
    if(ret != FTS_SUCCESS){
        DEBUG_FTS(("Fail send file"));
        goto out;
    }
    
    ret = FTS_SUCCESS;
out:
    return ret;
}

fts_result_t client_fts_socket_deinit(void)
{
    DEBUG_FTS(("Destroy socket"));
    return (close(sockfd_) < 0 ? FTS_ERROR : FTS_SUCCESS);
}
