/** @file */

#ifndef LIBDISCORD_0_3_LIBRARY_H
#define LIBDISCORD_0_3_LIBRARY_H

#include <curl/curl.h>
#include <libwebsockets.h>
#include <jansson.h>


#include "libdiscord_config.h"

#include "log.h"







#pragma mark Enumeration declarations




/**
 * @brief Return values from decoding
 * depreciated, use status.h LDS enums
 */
enum ldecode
{
    LDE_OK = 0,         
    LDE_LWS,            
    LDE_JSON,           
    LDE_CURL,           
    LDE_MEM,            
    LDE_MISSING_PARAM,  
    LDE_HB_ACKMISS,     
    LDE_HB_RINGBUF_FULL 
};

/**
 * @brief reasons included with user callback
 */
enum ld_callback_reason
{
    LD_CALLBACK_UNKNOWN = -1, 

   
    LD_CALLBACK_HELLO = 0,           
    LD_CALLBACK_RESUMED = 2,         
    LD_CALLBACK_INVALID_SESSION = 3, 

   
    LD_CALLBACK_READY = 1,                        
    LD_CALLBACK_CHANNEL_CREATE = 4,               
    LD_CALLBACK_CHANNEL_UPDATE = 5,               
    LD_CALLBACK_CHANNEL_DELETE = 6,               
    LD_CALLBACK_CHANNEL_PINS_UPDATE = 7,          
    LD_CALLBACK_GUILD_CREATE = 8,                 
    LD_CALLBACK_GUILD_UPDATE = 9,                 
    LD_CALLBACK_GUILD_DELETE = 10,                
    LD_CALLBACK_GUILD_BAN_ADD = 11,               
    LD_CALLBACK_GUILD_BAN_REMOVE = 12,            
    LD_CALLBACK_GUILD_EMOJIS_UPDATE = 13,         
    LD_CALLBACK_GUILD_INTEGRATIONS_UPDATE = 14,   
    LD_CALLBACK_GUILD_MEMBER_ADD = 15,            
    LD_CALLBACK_GUILD_MEMBER_REMOVE = 16,         
    LD_CALLBACK_GUILD_MEMBER_UPDATE = 17,         
    LD_CALLBACK_GUILD_MEMBERS_CHUNK = 18,         
    LD_CALLBACK_GUILD_ROLE_CREATE = 19,           
    LD_CALLBACK_GUILD_ROLE_UPDATE = 20,           
    LD_CALLBACK_GUILD_ROLE_DELETE = 21,           
    LD_CALLBACK_MESSAGE_CREATE = 22,              
    LD_CALLBACK_MESSAGE_UPDATE = 23,              
    LD_CALLBACK_MESSAGE_DELETE = 24,              
    LD_CALLBACK_MESSAGE_DELETE_BULK = 25,         
    LD_CALLBACK_MESSAGE_REACTION_ADD = 26,        
    LD_CALLBACK_MESSAGE_REACTION_REMOVE = 27,     
    LD_CALLBACK_MESSAGE_REACTION_REMOVE_ALL = 28, 
    LD_CALLBACK_PRESENCE_UPDATE = 29,             
    LD_CALLBACK_TYPING_START = 30,                
    LD_CALLBACK_USER_UPDATE = 31,                 
    LD_CALLBACK_VOICE_STATE_UPDATE = 32,          
    LD_CALLBACK_VOICE_SERVER_UPDATE = 33,         
    LD_CALLBACK_WEBHOOKS_UPDATE = 34,             

   
    LD_CALLBACK_WS_ESTABLISHED = 35,       
    LD_CALLBACK_WS_CONNECTION_ERROR = 36,  
    LD_CALLBACK_WS_GATEWAY_INIT_CLOSE = 37 /**
    the gateway closed the connection. len contains the close code,
    and data may or may not contain a string with a close message
    */

};

/**
 * @brief State of the gateway connection
 * depreciated
 */
enum ld_gateway_state
{
    LD_GATEWAY_UNCONNECTED = 0, 
    LD_GATEWAY_CONNECTING = 2,  
    LD_GATEWAY_CONNECTED = 3    
};

/**
 * @brief enum for opcodes that can we received from the gateway. 
 */
enum ld_gateway_opcode
{
    LD_GATEWAY_OPCODE_UNKNOWN = 999,          
    LD_GATEWAY_OPCODE_DISPATCH = 0,           
    LD_GATEWAY_OPCODE_HEARTBEAT = 1,          
    LD_GATEWAY_OPCODE_IDENTIFY = 2,           
    LD_GATEWAY_OPCODE_PRESENCE = 3,           
    LD_GATEWAY_OPCODE_VOICE_STATE = 4,        
    LD_GATEWAY_OPCODE_VOICE_PING = 5,         
    LD_GATEWAY_OPCODE_RESUME = 6,             
    LD_GATEWAY_OPCODE_RECONNECT = 7,          
    LD_GATEWAY_OPCODE_REQUEST_MEMBERS = 8,    
    LD_GATEWAY_OPCODE_INVALIDATE_SESSION = 9, 
    LD_GATEWAY_OPCODE_HELLO = 10,             
    LD_GATEWAY_OPCODE_HEARTBEAT_ACK = 11,     
    LD_GATEWAY_OPCODE_GUILD_SYNC = 12         
};

/**
 * @brief enums for the four possible fields inside the discord gateway
 *
 */
enum ld_gateway_payloadtype
{
    LD_GATEWAY_OP = 0,
    LD_GATEWAY_D = 1,
    LD_GATEWAY_T = 2,
    LD_GATEWAY_S = 3,
    LD_GATEWAY_UNKNOWN = 100
};

/**
 * @brief enum used to identify dispatch events
 * @todo identify events and put them here
 */
enum ld_dispatch_event
{
    LD_DISPATCH_UNKNOWN = 999,
    LD_PRESENCES_REPLACE 
};




#pragma mark Struct declarations






/**
 * @brief Gateway ringbuffer elements
 * 
 * Contains payload to be sent and metadata
 */
struct ld_gateway_payload
{
    void *payload; 
    size_t len;    
};

struct ld_gi;

struct ld_gateway_session
{
};

/**
 * 
 * @brief Context for each bot
 * 
 * one context can have multiple gateway (websocket) connections to discord
 *  sharding, voice connections
 * gateway_connected: are we connected to the gateway?
 * gateway_disconnected: were we disconnected to the gateway?
 * gateway_unconnected: have we ever connected to the gateway?
 * user_callback: user defined callback function for event loops.
 */
struct ld_context
{
    char *bot_token;                 
    void *user_data;                 
    unsigned long log_level;         
    char *gateway_url;               
    char *gateway_bot_url;           
    int shards;                      
    CURLM *curl_multi_handle;        
    CURL *curl_handle;               
    struct lws_context *lws_context; 
    struct lws *lws_wsi;             
    int (*user_callback)(struct ld_context *context, enum ld_callback_reason reason, void *data, int len);
    unsigned int heartbeat_interval; 
    int last_seq;                    
    unsigned long last_hb;           
    int hb_count;                    
    struct lws_ring *gateway_ring;   
    unsigned int close_code;         
    char *gateway_rx_buffer;         
    size_t gateway_rx_buffer_len;    
    
    char *gateway_session_id;          
    int gateway_bot_limit;             
    int gateway_bot_remaining;         
    unsigned long gateway_bot_reset;   
    struct ld_gi **gi;                 
    int gi_count;                      
    struct ld_json_user *current_user; 

    
    char *device;
    char *browser;
    char *os;

    struct ld_json_status_update *init_presence; 
};

/**
 * @brief libdiscord gateway connection information
 * 
 * One info struct per gateway connection (i.e. one per shard).
 * It is created and destroyed with each shard, **not** on disconnection,
 * but rather when a shard is closed.
 * 
 * (TODO: when is a shard closed?)
 */
struct ld_gi
{
    struct ld_context *parent_context; 
    void *user;                        
                                       
    int shardnum;                      
    struct lws *lws_wsi;               
    unsigned int hb_interval;          
    int last_seq;                      
    int hb_count;                      
    struct lws_ring *tx_ringbuffer;    
    unsigned int close_code;           
    int session_valid;                 
};

/**
 * @brief info used to generate a context
 * 
 * includes:
 *  bot token
 *  user-defined pointer to anything, can be metadata about the bot (creator, version, etc.)
 *  libdiscord logging level (see ld_log_level), DEPRECIATED, use logging functions in log.h instead
 */
struct ld_context_info
{
    char *bot_token;                                                                                       
    int (*user_callback)(struct ld_context *context, enum ld_callback_reason reason, void *data, int len); 
    size_t gateway_ringbuffer_size;                                                                        
    struct ld_json_status_update *init_presence;                                                           

    
    char *device;  
    char *browser; 
    char *os;      
};

/**
 * @brief ld_dispatch struct, used in dict
 */
struct ld_dispatch
{
    const char *name;                                                   
    enum ld_callback_reason cbk_reason;                                 
    int (*dispatch_callback)(struct ld_context *context, json_t *data); 
};


struct ld_json_identify;
struct ld_json_identify;




#pragma mark Function declarations




/**
 * @brief Initializes a ld_context_info struct
 * 
 * @param info A pointer to the ld_context_info to initialize
 * 
 * @return int Success code (0 success, 1 error)
 */
ld_status ld_init_context_info(struct ld_context_info *info);

/**
 * @brief Initializes an ld_context struct using a given ld_context_info
 * @details Takes a context info struct and creates a context with relevant info nessecary for the lifetime of a discord
 * bot.
 * @todo Move websocket lws_context initilization here, from ld_gateway_connect
 * 
 * @param info A pointer to an already-initialized ld_context_info
 * @param context A pointer to the context to initialize, with zeroed memory already allocated to it.
 * 
 * @return int Status code
 */
ld_status ld_init_context(const struct ld_context_info *info, struct ld_context *context);

/**
 * @brief Frees the members of a given ld_context struct, then destroys the struct
 * 
 * @param context A pointer to the ld_context to destroy
 */
void ld_cleanup_context(struct ld_context *context);

/**
 * @brief Private function hdah makes a GET request to /gateway/bot and retrieves shard number and gateway URL/determines bot token
 * is invalid. Uses the ulfius blocking REST interface.
 * @n internal /gateway/bot function
 *
 * @param context A pointer to the ld_context used for the query
 * 
 * @return int Status code
 * @n 0: success
 * @n 2: CURL error
 * @n 3: jansson (JSON parsing) error
 */
ld_status _ld_get_gateway_bot(struct ld_context *context);

/**
 * @brief curl callback function used to read data returned from HTTP request
 * 
 * @param contents Pointer to contents of HTTP response data
 * @param size 
 * @param nmemb 
 * @param userptr 
 * @return size_t Size of the response data
 */
size_t _ld_curl_response_string(void *contents, size_t size, size_t nmemb, void *userptr);

/**
 * @brief Private function that makes a GET request to /gateway and retrieves the gateway URL
 * used to determine if we can even connect to Discord, not _strictly_ nessecary
 * blocking.
 * @n (TODO: migrate to REST.h/REST.c)
 * @n internal GET /gateway function
 * 
 * @param context Pointer to ld_context struct 
 * 
 * @return int Status code
 * @n 0: success
 * @n 1: ulfius error
 * @n 2: CURL error
 * @n 3: jansson (JSON parsing) error
 */
ld_status _ld_get_gateway(struct ld_context *context);

/**
 * @brief CURL callback function used to (currently) print out HTTP headers line by line for /gateway and /gateway/bot
 * 
 * @param buffer 
 * @param size 
 * @param nitems 
 * @param userdata 
 * @return size_t 
 */
size_t ld_curl_header_parser(char *buffer, size_t size, size_t nitems, void *userdata);

/**
 * @brief Sets identify information in given ld_context struct
 * 
 * @param context Pointer to ld_context struct in which to store identify information
 * @param identify Pointer to ld_json_identify struct, from which to take identify information
 * @return int Status code
 */
ld_status ld_set_identify(struct ld_context *context, struct ld_json_identify *identify);

/**
 * @brief Connects to Discord with a given ld_context struct
 * 
 * @param context Pointer to an ld_context struct
 * @return int Status code
 */
ld_status ld_connect(struct ld_context *context);

/**
 * @brief Checks services pending HTTP and websocket requests
 * @n checks if the heartbeat timer is up
 * 
 * @param context 
 * @param timeout 
 * 
 * @return int Status code
 * @n 0: OK
 * @n 1: websocket ringbuffer error
 */
ld_status ld_service(struct ld_context *context, int timeout);

/**
 * @brief Starts a fresh gateway connection
 * @details Called inside ld_connect, initializes and creates a libwebsocket client connection
 *
 * @todo allocate lws_client_connect_info on stack instead of heap
 * 
 * @param context Pointer to an ld_context struct
 * 
 * @return int Status code. 0 (LDS_OK) on success.
 */
ld_status ld_gateway_connect(struct ld_context *context);

/**
 * @brief picks out interesting callback reasons (like receive and writeable) and does stuff 
 * responsible for connecting/disconnecting from the gateway, receiving payloads, and sending payloads
 * 
 * @param wsi 
 * @param lws_callback_reasons 
 * @param user 
 * @param in 
 * @param len 
 * 
 * @return int 
 */
ld_status ld_lws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                          void *user, void *in, size_t len);

/**
 * @brief 
 * 
 * @param context 
 * @param identify 
 * @return int 
 */
ld_status ld_set_identify(struct ld_context *context, struct ld_json_identify *identify);

/**
 * @brief 
 * 
 * @param identify 
 * @return int 
 */
ld_status ld_cleanup_identify(struct ld_json_identify *identify);

/**
 * @brief 
 * 
 * @param context 
 * @param in 
 * @param len 
 * 
 * @return int 
 */
ld_status ld_gateway_payload_parser(struct ld_context *context, char *in, size_t len);

/**
 * @brief callback for parsing READY dispatches, saves session_id for resuming
 * 
 * @param context Pointer to ld_context for which to parse READY dispatches
 * @param data Pointer to JSON object containing dispatch data
 * 
 * @return int Status code
 */
ld_status ld_dispatch_ready(struct ld_context *context, json_t *data);

/**
 * @brief 
 *
 * @param context Pointer to ld_context for which to parse gateway dispatch
 * @param type String object representing the dispatch type
 * @param data Pointer to JSON object containing dispatch data
 * 
 * @return int Status code
 * @n LDS_OK: success
 * @n 1: jansson (JSON parsing) error
 */
ld_status ld_gateway_dispatch_parser(struct ld_context *context, json_t *type, json_t *data);

/**
 * @brief Queues a heartbeat payload into the websocket tx ringbuffer
 * 
 * @param context Pointer to an ld_context for which to queue the payload
 * @return int Status code
 */
ld_status ld_gateway_queue_heartbeat(struct ld_context *context);

/**
 * @brief Gets the operating system name. Depreciated, use CMake's CMAKE_SYSTEM_NAME in libdiscord_config.h.in
 * 
 * @return char* A string containing the operating system name
 */
char *ld_get_os_name();

#include "REST.h"
#include "json.h"

#endif