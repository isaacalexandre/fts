#ifndef _LIB_FTS_CLIENT_H_
#define _LIB_FTS_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "libftsserver.h"
#include "publictypedef.h"
#include <sys/types.h>
#include <sys/socket.h>

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

/**
 * @brief
 * Create a handle to a Socket
 * @param[in,out] fd
 * Handler number of the socket
 * @return Returns FTS_SUCCESS if the function succeeds.
 */
fts_result_t socket_create(int* fd, int port);


#ifdef __cplusplus
}
#endif

#endif /* _LIB_FTS_CLIENT_H_ */
