/** @file */

#ifndef LIBDISCORD_STATUS_H
#define LIBDISCORD_STATUS_H

/**
 * @brief Status codes returned from libdiscord functions
 * @description
 * 0 to 99: OK, error, and memory issues
 * 100 to 199: generic connection issues
 * 200 to 299: libwebsocket issues
 * 300 to 399: JSON parsing issues (jansson, etc)
 * 900 to 999: Discord issues (tokens, permissions, etc.)
 */
typedef enum ld_status_enum
{
    LDS_OK = 0,
    LDS_ERROR = 1,
    LDS_MEMORY_ERR = 2,
    LDS_ALLOC_ERR = 3,
    LDS_INCOMPLETE_ARGS_ERR = 4,
    LDS_CONNECTION_ERR = 100,
    LDS_CURL_ERR = 101,
    LDS_WEBSOCKET_RINGBUFFER_ERR = 200,
    LDS_WEBSOCKET_RINGBUFFER_FULL_ERR = 201,
    LDS_WEBSOCKET_CANTFIT_PAYLOAD_ERR = 210,
    LDS_WEBSOCKET_CANTFIT_HEARTBEAT_ERR = 211,
    LDS_WEBSOCKET_HEARTBEAT_ACKNOWLEDEGEMENT_MISSED = 220,
    LDS_WEBSOCKET_INIT_ERR = 230,
    LDS_JSON_ERR = 300,
    LDS_JSON_DECODING_ERR = 301,
    LDS_JSON_ENCODING_ERR = 302,
    LDS_JSON_DUMP_ERR = 303,
    LDS_JSON_INVALID_ERR,
    LDS_JSON_CANTFINDKEY_ERR = 310,
    LDS_JSON_CANTFINDVALUE_ERR = 311,
    LDS_JSON_MISSING_REQUIRED_ERR = 312,
    LDS_JSON_GATEWAY_DISPATCH_TYPE_ERR = 320,
    LDS_TOKEN_MISSING_ERR = 900
} ld_status;

#endif