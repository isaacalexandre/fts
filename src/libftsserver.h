#ifndef _LIB_FTS_CLIENT_H_
#define _LIB_FTS_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "publictypedef.h"

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
 * Set lib debug
 * @param[in] enabled
 * Select debug
 */
void server_fts_debug_enable(bool enabled);

/**
 * @brief
 * Create a handle to a Socket
 * @param[in] port
 * Socket port number
 * @param[in] ip_server
 * Socket IP number
 * @return Returns FTS_SUCCESS if the function succeeds.
 */
fts_result_t server_fts_socket_init(uint16_t port, uint32_t ip_server);

/**
 * @brief
 * Call to receive file from the client
 * @param[in] path
 * Path in the system to receive the file.
 * @return Returns FTS_SUCCESS if the function succeeds.
 */
fts_result_t server_fts_process_receive_file(const char* path);

/**
 * @brief
 * Destroy the socket created
 * @return Returns FTS_SUCCESS if the function succeeds.
 */
fts_result_t server_fts_socket_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIB_FTS_CLIENT_H_ */
