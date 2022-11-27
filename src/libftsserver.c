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
#define BUFFER_SIZE     (2048)
#define PATH_FILE_SIZE  (1000)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef struct {
    char* path;
    size_t path_buffer_size;
    size_t size_file;
    uint32_t crc;
} file_header_params_t;

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

static fts_result_t server_fts_verify_crc_(char* path, uint32_t crc)
{
    uint32_t crc_file = 0;

    //Calcule CRC of the file
    DEBUG_FTS(("Calcule CRC32 of the file at path: [%s]", path));
    if( file_crc32(&crc_file, path) == false)
    {
        DEBUG_FTS(("Fail calculate CRC32"));
        return FTS_PARTIAL_RESULTS;
    }
    DEBUG_FTS(("CRC32 FILE: 0x%08X",crc_file));
    DEBUG_FTS(("CRC32 PARAM: 0x%08X",crc));

    return (crc_file == crc ? FTS_SUCCESS : FTS_BADVALUE);
}

#define NUM_GET_STR_SIZE 20
static fts_result_t server_fts_recv_header_(char* buffer, file_header_params_t* st_headers)
{
    fts_result_t ret = FTS_ERROR;
    char filename[PATH_FILE_SIZE] = {'\0'};
    char aux_num[NUM_GET_STR_SIZE] = {'\0'};
    size_t size_path = strlen(st_headers->path);

    //Sanity
    if(buffer == NULL || st_headers == NULL){
        DEBUG_FTS(("Buffer pointer is null"));
        ret = FTS_BADARG;
        goto out;
    }
    if(st_headers->path == NULL){
        DEBUG_FTS(("Path pointer is null"));
        ret = FTS_BADARG;
        goto out;
    }

    //Get info from buffer the variables
    //Filename
    for (size_t i = 0; i < PATH_FILE_SIZE; i++) {
        if(*(buffer) != '/') {
            filename[i] = *buffer;
        } else {
            filename[i] = '\0';
            buffer++;
            break;
        }
        buffer++;
    }
    DEBUG_FTS(("Header Path: %s", filename));

    //Size File
    for (size_t i = 0; i < NUM_GET_STR_SIZE; i++) {
        if(*(buffer) != '/') {
            aux_num[i] = *buffer;
        } else {
            aux_num[i] = '\0';
            buffer++;
            break;
        }
        buffer++;
    }
    st_headers->size_file = atol(aux_num);
    DEBUG_FTS(("Header Size File: 0x%08X", (unsigned int)st_headers->size_file));

    //CRC - Copy rest of the buffer
    strcpy(aux_num,buffer);
    st_headers->crc = atoi(aux_num);
    DEBUG_FTS(("Header CRC: 0x%08X bytes", (unsigned int)st_headers->crc));

    //Verify size of path and filename
    if(strlen(filename) + size_path > st_headers->path_buffer_size){
        DEBUG_FTS(("Size Filename + Path oversized buffer"));
        ret = FTS_BADVALUE;
        goto out;
    }

    //If path do not have slash, need be insert
    if( size_path > 0 && is_slash_end_str(st_headers->path) == false)
        strcat(st_headers->path, "/");
    
    //Concat path with filenmae
    strcat(st_headers->path, filename);

    ret = FTS_SUCCESS;
out:
    return ret;
}

static fts_result_t server_fts_process_rcv_(file_header_params_t* st_headers)
{
    int loopfd = 0;
    int rcv_data_len = 0;
    uint8_t buffer[BUFFER_SIZE] = {0};
    FILE* p_file = NULL;
    fts_result_t ret = FTS_ERROR;

    //Accept request
    DEBUG_FTS(("Call accept"));
    loopfd = accept(sockfd_, NULL, NULL);
    DEBUG_FTS(("Accepted - Socket FD: [%d]", sockfd_));

    //Receive file name
    DEBUG_FTS(("Call RECV"));
    rcv_data_len = recv(loopfd, buffer, BUFFER_SIZE, 0);
    DEBUG_FTS(("Received %d bytes", rcv_data_len));
    //Print buffer
    DEBUG_FTS_FRAME(buffer, rcv_data_len);

    //Set headers
    ret = server_fts_recv_header_((char*)buffer, st_headers);
    if(ret != FTS_SUCCESS){
        DEBUG_FTS(("Fail parse header"));
        //Send fail to client
        send(loopfd, "0", 1 , 0);
        goto out;
    }
    DEBUG_FTS(("Call SEND - Header OK"));
    send(loopfd, "1", 1 , 0);

    //Create file
    DEBUG_FTS(("Open file at path: [%s]", st_headers->path));
    p_file = fopen(st_headers->path,"w");
    if (p_file == NULL){
        DEBUG_FTS(("Error creating file"));
        ret = FTS_OPEN_FILE_ERROR;
        goto out;
    }

    do{
        DEBUG_FTS(("Call RECV"));
        rcv_data_len = recv(loopfd, buffer, BUFFER_SIZE, 0);
        fwrite(buffer, rcv_data_len, 1, p_file);
        DEBUG_FTS(("Received %d bytes", rcv_data_len));
        //Print buffer
        DEBUG_FTS_FRAME(buffer, rcv_data_len);
        DEBUG_FTS(("Call SEND"));
        send(loopfd, "1", 1 , 0);
    }while(rcv_data_len > 0);

    ret= FTS_SUCCESS;
out:
    //Close the file
    fclose(p_file);
    //Close the session socket
    close(loopfd);

    return ret;
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
    file_header_params_t st_file;
    char path_local[PATH_FILE_SIZE] = {0};
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

    //Verify if fit the path
    if(strlen(path) > PATH_FILE_SIZE) {
        DEBUG_FTS(("Path size is bigger then buffer"));
        ret = FTS_BADARG;
        goto out;
    }

    //Set variables
    st_file.path = (char*)path_local;
    st_file.path_buffer_size = PATH_FILE_SIZE;
    
    while(1){
        //Copy path to local path
        strcpy(st_file.path, path);

        //Process receive file
        ret = server_fts_process_rcv_(&st_file);
        if(ret != FTS_SUCCESS){
            DEBUG_FTS(("Error process receive file"));
            goto out;
        }

        //Verify CRC
        ret = server_fts_verify_crc_(st_file.path, st_file.crc);
        //If the CRC not match, delete the file    
        if(ret != FTS_SUCCESS) {
            DEBUG_FTS(("Delete file: [%s]", st_file.path));
            remove(st_file.path);
        }
    }
    
out:
    return ret;
}

fts_result_t server_fts_socket_deinit(void)
{
    DEBUG_FTS(("Destroy socket"));
    return (close(sockfd_) < 0 ? FTS_ERROR : FTS_SUCCESS);
}
