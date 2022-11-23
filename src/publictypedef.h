#ifndef _LIB_PUBLIC_TYPEDEF_H_
#define _LIB_PUBLIC_TYPEDEF_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/
typedef enum
{
        FTS_SUCCESS                      =  0 ,  /**< Success */
        FTS_PENDING                      =  1 ,  /**< Pending */
        FTS_TIMEOUT                      =  2 ,  /**< Timeout */
        FTS_PARTIAL_RESULTS              =  3 ,  /**< Partial results */
        FTS_ERROR                        =  4 ,  /**< Error */
        FTS_BADARG                       =  5 ,  /**< Bad Arguments */
        FTS_BADOPTION                    =  6 ,  /**< Mode not supported */
        FTS_UNSUPPORTED                  =  7 ,  /**< Unsupported function */
        FTS_OUT_OF_HEAP_SPACE            =  8 ,  /**< Dynamic memory space exhausted */
        FTS_NOTUP                        =  9 ,  /**< Interface is not currently Up */
        FTS_UNFINISHED                   = 10 ,  /**< Operation not finished yet */
        FTS_CONNECTION_LOST              = 11 ,  /**< Connection to server lost */
        FTS_NOT_FOUND                    = 12 ,  /**< Item not found */
        FTS_PACKET_BUFFER_CORRUPT        = 13 ,  /**< Packet buffer corrupted */
        FTS_ROUTING_ERROR                = 14 ,  /**< Routing error */
        FTS_BADVALUE                     = 15 ,  /**< Bad value */
        FTS_WOULD_BLOCK                  = 16 ,  /**< Function would block */
        FTS_ABORTED                      = 17 ,  /**< Operation aborted */
        FTS_CONNECTION_RESET             = 18 ,  /**< Connection has been reset */
        FTS_CONNECTION_CLOSED            = 19 ,  /**< Connection is closed */
        FTS_NOT_CONNECTED                = 20 ,  /**< Connection is not connected */
        FTS_ADDRESS_IN_USE               = 21 ,  /**< Address is in use */
        FTS_NETWORK_INTERFACE_ERROR      = 22 ,  /**< Network interface error */
        FTS_ALREADY_CONNECTED            = 23 ,  /**< Socket is already connected */
        FTS_INVALID_INTERFACE            = 24 ,  /**< Interface specified in invalid */
        FTS_SOCKET_CREATE_FAIL           = 25 ,  /**< Socket creation failed */
        FTS_INVALID_SOCKET               = 26 ,  /**< Socket is invalid */
        FTS_CORRUPT_PACKET_BUFFER        = 27 ,  /**< Packet buffer is corrupted */
        FTS_UNKNOWN_NETWORK_STACK_ERROR  = 28 ,  /**< Unknown network stack error */
        FTS_BIND_ERROR                   = 29 ,  /**< Error bind create */
        FTS_LISTEN_ERROR                 = 30 ,  /**< Listen socket fail */
        FTS_PACKET_BUFFER_OVERFLOW       = 31 ,  /**< Packet buffer overflow */
        FTS_ALREADY_INITIALIZED          = 32 ,  /**< Module has already been inited */
        FTS_EMPTY_EMAIL_LIST             = 33 ,  /**< Email list is empty */
        FTS_EMPTY_RECEIVER_FLAG          = 34 ,  /**< Receiver flag is empty */
        FTS_BUSY                         = 35 ,  /**< Busy */
        FTS_UNAUTHORIZED                 = 36 ,  /**< Unauthorized */
        FTS_FORBIDDEN                    = 37 ,  /**< Forbidden */
        FTS_PRECONDITION_FAILED          = 38 ,  /**< Precondition failed */
        FTS_CONFLICT                     = 49 ,  /**< Conflict */
}fts_result_t;

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

#ifdef __cplusplus
}
#endif

#endif /* _PUBLIC_TYPEDEF_H_ */