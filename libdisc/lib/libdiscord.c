#include <stdlib.h>
#include <jansson.h>

#include "libdiscord.h"
#include "log.h"

static struct lws_protocols protocols[] = {
    {"DiscordBot",
     (int (*)(struct lws *, enum lws_callback_reasons, void *, void *, size_t))ld_lws_callback,
     8192,
     8192},
    {NULL, NULL, 0

    }};

static const struct lws_extension exts[] = {
    {"permessage-deflate",
     lws_extension_callback_pm_deflate,
     "permessage-deflate; client_no_context_takeover; client_max_window_bits"},
    {NULL, NULL, NULL}};

ld_status ld_init_context_info(struct ld_context_info *info)
{
    if (info == NULL)
    {
        ld_warning("ld_init_context_info: recieved null pointer");
        return 1;
    }

    info->bot_token = NULL;
    info->user_callback = NULL;
    info->gateway_ringbuffer_size = 16;
    info->init_presence = NULL;

    info->device = NULL;
    info->browser = NULL;
    info->os = NULL;

    return 0;
}

ld_status ld_init_curl(const struct ld_context_info *info, struct ld_context *context)
{

    curl_global_init(CURL_GLOBAL_DEFAULT);
    context->curl_multi_handle = curl_multi_init();
    context->curl_handle = curl_easy_init();
    if (context->curl_handle == NULL)
    {
        ld_error("%s: curl_easy_init() returned NULL", __FUNCTION__);
        return LDS_CURL_ERR;
    }
    return LDS_OK;
}

ld_status ld_init_lws(const struct ld_context_info *info, struct ld_context *context)
{

    context->lws_context = NULL;
    struct lws_context_creation_info ccinfo;
    memset(&ccinfo, 0, sizeof(struct lws_context_creation_info));

    ccinfo.port = CONTEXT_PORT_NO_LISTEN;
    ccinfo.iface = NULL;
    ccinfo.protocols = protocols;
    ccinfo.extensions = exts;
    ccinfo.options = LWS_SERVER_OPTION_VALIDATE_UTF8 | LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    ccinfo.ssl_cert_filepath = NULL;
    ccinfo.ssl_private_key_filepath = NULL;
    ccinfo.gid = -1;
    ccinfo.uid = -1;
    ccinfo.server_string = NULL;
    ccinfo.user = context;

    context->lws_context = lws_create_context(&ccinfo);
    if (context->lws_context == NULL)
    {
        ld_error("%s: couldn't create lws_context\n", __FUNCTION__);
        return LDS_WEBSOCKET_INIT_ERR;
    }

    context->gateway_ring = lws_ring_create(
        sizeof(struct ld_gateway_payload),
        (info == NULL) ? LD_RINGBUFFER_DEFAULT_SIZE : info->gateway_ringbuffer_size,
        NULL);
    if (context->gateway_ring == NULL)
    {
        ld_error("couldn't init gateway ringbuffer");
        return LDS_WEBSOCKET_RINGBUFFER_ERR;
    }

    return LDS_OK;
}

ld_status ld_init_context(const struct ld_context_info *info, struct ld_context *context)
{
    if (context == NULL)
    {

        ld_error("%s: was passed null pointer for context", __FUNCTION__);
        return LDS_MEMORY_ERR;
    }

    memset(context, 0, sizeof(struct ld_context));

    ld_status ret;

    ret = ld_init_curl(info, context);
    if (ret != LDS_OK)
    {
        ld_error("%s: curl init error (%d)", __FUNCTION__, ret);
        return ret;
    }

    lws_set_log_level(ld_get_logging_level(), NULL);

    ret = ld_init_lws(info, context);
    if (ret != LDS_OK)
    {
        ld_error("%s: lws init error (%d)", __FUNCTION__, ret);
        return ret;
    }

    context->gateway_bot_limit = 1;
    context->gateway_bot_remaining = 1;
    context->gateway_bot_reset = lws_now_secs();

    context->gi_count = 0;

    context->heartbeat_interval = 0;
    context->hb_count = 0;
    context->last_hb = lws_now_secs();

    context->gateway_rx_buffer_len = 0;
    context->gateway_rx_buffer = NULL;

    context->gateway_session_id = NULL;

    context->current_user = NULL;

    context->gateway_session_id = NULL;

    context->init_presence = NULL;

    if (info != NULL && info->device != NULL)
    {
        context->device = strdup(info->device);
    }
    else
    {
        context->device = LD_LIBNAME;
    }

    if (info != NULL && info->browser != NULL)
    {
        context->browser = strdup(info->browser);
    }
    else
    {
        context->browser = LD_LIBNAME;
    }

    if (info != NULL && info->os != NULL)
    {
        context->os = strdup(info->browser);
    }
    else
    {
        context->os = LD_SYSTEMOS;
    }

    if (info == NULL)
    {
        ld_notice("%s: initializing without context info. Many libdiscord functions will be unusable", __FUNCTION__);
        return LDS_OK;
    }

    context->user_callback = info->user_callback;

    if (info->bot_token == NULL)
    {
        ld_error("%s: bot token is null, can't continue", __FUNCTION__);
        return LDS_TOKEN_MISSING_ERR;
    }
    context->bot_token = strdup(info->bot_token);

    context->init_presence = malloc(sizeof(struct ld_json_status_update));
    if (context->init_presence == NULL)
    {
        ld_error("%s: error mallocing initial presence");
        return LDS_MEMORY_ERR;
    }
    if (info->init_presence != NULL)
        context->init_presence = memcpy(context->init_presence, info->init_presence, sizeof(struct ld_json_status_update));
    else
        context->init_presence = NULL;

    return LDS_OK;
}

void ld_cleanup_context(struct ld_context *context)
{

    if (context == NULL)
    {
        return;
    }

    if (context->gateway_session_id != NULL)
    {
        free(context->gateway_session_id);
    }
    if (context->current_user != NULL)
    {
        free(context->current_user);
    }

    if (context->init_presence != NULL)
    {
        free(context->init_presence);
    }
    curl_multi_cleanup(context->curl_multi_handle);
    curl_global_cleanup();
    lws_context_destroy(context->lws_context);
    lws_ring_destroy(context->gateway_ring);
}

struct _ld_buffer
{
    char *string;
    size_t size;
    struct ld_context *context;
};

size_t _ld_curl_response_string(void *contents, size_t size, size_t nmemb, void *userptr)
{
    size_t recieved_size = size * nmemb;
    struct _ld_buffer *buffer = (struct _ld_buffer *)userptr;

    buffer->string = realloc(buffer->string, buffer->size + recieved_size + 1);
    if (buffer->string == NULL)
    {
        ld_error("realloc: couldn't allocate memory for curl response string in ld_connect!");
    }

    memcpy(&(buffer->string[buffer->size]), contents, recieved_size);
    buffer->size += recieved_size;
    buffer->string[buffer->size] = '\0';

    return recieved_size;
}

ld_status _ld_get_gateway(struct ld_context *context)
{

    int ret;
    struct ld_rest_request request;
    ld_rest_init_request(&request, NULL);
    struct ld_rest_response response;
    ld_rest_init_response(&response);

    ld_get_gateway(&request, context);

    ret = ld_rest_send_request(context, &response, &request);

    if (ret != LDE_OK)
    {
        ld_error("_ld_get_gateway: couldn't send request to /gateway (%d)", ret);
        return LDS_ERROR;
    }

    ld_debug("get gateway response: %s", response.body);

    json_t *object, *tmp;
    json_error_t error;

    object = json_loads(response.body, 0, &error);
    if (object == NULL)
    {
        ld_error("jansson: couldn't decode string returned "
                 "from /gateway in ld_connext: %s",
                 response.body);
        return LDS_JSON_DECODING_ERR;
    }

    tmp = json_object_get(object, "url");
    if (tmp == NULL)
    {
        ld_error("jansson: couldn't find key \"url\" in JSON object from /gateway");
        return LDS_JSON_ERR;
    }

    if (json_string_value(tmp) == NULL)
    {
        ld_error("jansson: didn't receive string object from "
                 "JSON payload received from gateway");
        return LDS_JSON_ERR;
    }

    context->gateway_url = malloc(strlen(json_string_value(tmp)) + 1);
    context->gateway_url = strcpy(context->gateway_url, json_string_value(tmp));

    json_decref(tmp);
    json_decref(object);
    return LDS_OK;
}

size_t ld_curl_header_parser(char *buffer, size_t size, size_t nitems, void *userdata)
{
    char *tmp;

    if (size * nitems == 2)
    {
        return 2;
    }

    tmp = strndup(buffer, size * nitems - 1);
    ld_debug("headers(%d): %s", (int)nitems * size, tmp);
    free(tmp);
    return size * nitems;
}

ld_status _ld_get_gateway_bot(struct ld_context *context)
{

    int ret;
    struct ld_rest_request request;
    struct ld_rest_response response;

    ld_rest_init_request(&request, NULL);
    ld_rest_init_response(&response);

    ld_get_gateway_bot(context, &request);

    ret = ld_rest_send_request(context, &response, &request);
    if (ret != LDS_OK)
    {
        ld_error("%s, couldn't send request to /gateway/bot", __FUNCTION__);
    }

    ld_debug("_ld_get_gateway_bot: get gateway bot response: %s", response.body);

    json_t *object, *tmp;
    json_error_t error;

    object = json_loads(response.body, 0, &error);
    if (object == NULL)
    {
        ld_error("_ld_get_gateway_bot: couldn't decode string returned "
                 "from /gateway/bot in ld_connect: %s",
                 response.body);
        return LDS_JSON_DECODING_ERR;
    }

    tmp = json_object_get(object, "url");
    if (tmp == NULL)
    {
        ld_error("_ld_get_gateway_bot: couldn't find key \"url\" in JSON object from /gateway/bot."
                 " is the bot token valid? are we being ratelimited?");
        return LDS_JSON_CANTFINDKEY_ERR;
    }

    const char *tmpurl;
    tmpurl = json_string_value(tmp);
    if (tmpurl == NULL)
    {
        ld_error("_ld_get_gateway_bot: didn't receive string object in \"url\" from "
                 "JSON payload received from /gateway/bot");
        return LDS_JSON_CANTFINDVALUE_ERR;
    }

    context->gateway_bot_url = malloc(strlen(tmpurl) + 1);
    context->gateway_bot_url = strcpy(context->gateway_bot_url, tmpurl);

    ld_debug("%s: gateway_bot_url: %s", __FUNCTION__, context->gateway_bot_url);

    tmp = json_object_get(object, "shards");
    if (tmp == NULL)
    {
        ld_error("jansson: couldn't find key \"shards\" in JSON object from /gateway/bot."
                 "is the bot token valid?");
        return LDS_JSON_CANTFINDKEY_ERR;
    }

    if (json_integer_value(tmp) == 0)
    {
        ld_error("jansson: didn't receive integer object in \"shards\" from "
                 "JSON payload received from /gateway/bot");
        return LDS_JSON_CANTFINDVALUE_ERR;
    }

    context->shards = (int)json_integer_value(tmp);
    ld_info("shards: %d", context->shards);
    return LDS_OK;
}

struct ld_gi *_ld_init_gi(struct ld_context *context)
{

    context->gi = malloc(sizeof(struct ld_gi));
    if (context->gi == NULL)
    {
        ld_error("couldn't allocate memory for gateway interface struct");
        return NULL;
    }

    struct ld_gi *gi = context->gi[0];

    gi->parent_context = context;

    gi->shardnum = -1;
    gi->lws_wsi = NULL;
    gi->hb_interval = 41500;
    gi->last_seq = 0;
    gi->hb_count = 0;
    gi->tx_ringbuffer = NULL;
    gi->close_code = 0;
    gi->session_valid = 0;

    return gi;
}

ld_status ld_connect(struct ld_context *context)
{
    int ret;

    if (context->gateway_url == NULL)
    {
        ret = _ld_get_gateway(context);
        if (ret != LDS_OK)
        {
            ld_error("ld_connect: couldn't get gateway URL from /gateway");
            return ret;
        }
    }

    if (context->gateway_bot_url == NULL)
    {
        ret = _ld_get_gateway_bot(context);
        if (ret != LDS_OK)
        {
            ld_error("ld_connect: couldn't get gateway URL from /gateway/bot");
            return ret;
        }
    }

    ld_debug("%s: gateway_bot_url: %s", __FUNCTION__, context->gateway_bot_url);

    if (context->current_user == NULL)
    {
        context->current_user = malloc(sizeof(struct ld_json_user));
        if (context->current_user == NULL)
        {
            ld_error("ld_connect: couldn't allocate user struct");
            return LDS_MEMORY_ERR;
        }
        ret = ld_get_current_user(context, context->current_user);
        if (ret != LDS_OK)
        {
            ld_error("ld_connect: couldn't get current user info from /users/@me");
            return ret;
        }
    }

    ret = ld_gateway_connect(context);

    if (ret != LDS_OK)
    {
        ld_warning("ld_connect: ld_gateway_connect returned bad");
        return LDS_WEBSOCKET_RINGBUFFER_ERR;
    }

    return LDS_OK;
}

ld_status ld_service(struct ld_context *context, int timeout)
{
    ld_status ret = 0;

    if (context->lws_wsi == NULL)
    {

        ret = ld_connect(context);
        if (ret != LDS_OK)
        {
            ld_notice("%s: ld_connect returned error, will retry in %d secs", __FUNCTION__, LD_CONNECT_DELAY_INTERVAL);
            sleep(LD_CONNECT_DELAY_INTERVAL);
            return LDS_CONNECTION_ERR;
        }
        return LDS_OK;
    }

    if (((lws_now_secs() - context->last_hb) > (context->heartbeat_interval / 1000)) &&
        context->heartbeat_interval != 0)
    {

        context->hb_count++;
        if (context->hb_count > 1)
        {

            ld_warning("%s: didn't recieve a HB_ACK", __FUNCTION__);

            return LDS_WEBSOCKET_HEARTBEAT_ACKNOWLEDEGEMENT_MISSED;
        }
        ret = ld_gateway_queue_heartbeat(context);
        if (ret != LDS_OK)
        {
            ld_warning("couldn't put heartbeat into gateway tx ringbuffer");
            context->hb_count--;
            return LDS_WEBSOCKET_CANTFIT_HEARTBEAT_ERR;
        }

        context->last_hb = lws_now_secs();
    }

    if (lws_ring_get_count_waiting_elements(context->gateway_ring, NULL) != 0)
        lws_callback_on_writable(context->lws_wsi);

    lws_service(context->lws_context, timeout);
    return 0;
}

ld_status ld_gateway_connect(struct ld_context *context)
{

    struct lws_client_connect_info *i;

    i = malloc(sizeof(struct lws_client_connect_info));
    memset(i, 0, sizeof(struct lws_client_connect_info));

    i->context = context->lws_context;

    char *gateway_url;
    gateway_url = malloc(1000);
    sprintf(gateway_url, "/?v=%d&encoding=json", LD_WS_API_VERSION);
    i->address = context->gateway_bot_url + 6;

    i->port = 443;
    i->ssl_connection = 1;
    i->path = gateway_url;

    char *ads_port;
    ads_port = malloc((strlen(i->address) + 10) * sizeof(char));
    sprintf(ads_port, "%s:%u", i->address, 443 & 65535);
    i->host = ads_port;
    i->origin = ads_port;

    i->protocol = protocols[0].name;

    i->pwsi = &context->lws_wsi;

    ld_debug("ld_gateway_connect: connecting to gateway");

    lws_client_connect_via_info(i);
    if (context->lws_wsi == NULL)
    {
        ld_error("ld_gateway_connect: failed to connect to gateway (%s)", i->address);
        return 1;
    }
    free(ads_port);
    free(i);
    return 0;
}

ld_status ld_lws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                          void *user, void *in, size_t len)
{

    struct ld_context *context;
    context = lws_context_user(lws_get_context(wsi));
    int i;
    char *payload = (char *)user;
    struct ld_gateway_payload *gateway_payload;
    char *close_message = NULL;
    switch (reason)
    {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        ld_error("lws: error connecting to gateway: %.*s(%d)", in, len, len);

        i = context->user_callback(context, LD_CALLBACK_WS_CONNECTION_ERROR, NULL, 0);
        context->lws_wsi = NULL;
        return i;
    case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
        ld_info("lws: received handshake from Discord gateway");
        return 0;
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        ld_info("established websocket connection to gateway");
        i = context->user_callback(context, LD_CALLBACK_WS_ESTABLISHED, NULL, 0);

        context->last_hb = lws_now_secs();
        return i;

    case LWS_CALLBACK_GET_THREAD_ID:
        return 0;
    case LWS_CALLBACK_CLOSED:
        ld_notice("lws: websocket connection to gateway closed");

        break;
    case LWS_CALLBACK_CLIENT_RECEIVE:

        if (context->gateway_rx_buffer == NULL && lws_is_final_fragment(wsi))
        {

            i = ld_gateway_payload_parser(context, in, len);

            ld_debug("single RX: %s", (char *)in);
            context->gateway_rx_buffer = NULL;
            context->gateway_rx_buffer_len = 0;

            return i;
        }

        if (lws_is_first_fragment(wsi))
        {

            context->gateway_rx_buffer = malloc(len + 1);
            strncpy(context->gateway_rx_buffer, in, len);
            context->gateway_rx_buffer_len = len;

            return 0;
        }
        if (lws_is_final_fragment(wsi) && context->gateway_rx_buffer != NULL)
        {

            context->gateway_rx_buffer = realloc(context->gateway_rx_buffer,
                                                 context->gateway_rx_buffer_len + len + 2);

            strncpy(context->gateway_rx_buffer + context->gateway_rx_buffer_len, in, len);

            context->gateway_rx_buffer_len += len;

            context->gateway_rx_buffer[context->gateway_rx_buffer_len] = '\0';

            if (context->gateway_rx_buffer_len > 2000)
            {
                ld_debug("multi RX: %.*s...", 2000, context->gateway_rx_buffer);
            }
            else
            {
                ld_debug("multi RX: %s", context->gateway_rx_buffer);
            }

            i = ld_gateway_payload_parser(context, context->gateway_rx_buffer, context->gateway_rx_buffer_len);

            free(context->gateway_rx_buffer);
            context->gateway_rx_buffer = NULL;
            context->gateway_rx_buffer_len = 0;

            return i;
        }

        context->gateway_rx_buffer = realloc(context->gateway_rx_buffer,
                                             context->gateway_rx_buffer_len + len + 1);
        strncpy(context->gateway_rx_buffer + context->gateway_rx_buffer_len, in, len);
        context->gateway_rx_buffer_len += len;

        return 0;
    case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
        ld_debug("ld_lws_callback: recieved websocket pong from gateway");
        break;
    case LWS_CALLBACK_CLIENT_WRITEABLE:
        ld_debug("lws: client writable callback");
        gateway_payload = malloc(sizeof(struct ld_gateway_payload));

        if (lws_ring_get_count_waiting_elements(context->gateway_ring, NULL) == 0)
        {
            ld_debug("nothing in queue to send");
            break;
        }

        i = (int)lws_ring_consume(context->gateway_ring, NULL, gateway_payload, 1);
        if (i != 1)
        {
            ld_warning("couldn't consume payload from ringbuffer");
            break;
        }

        i = sprintf(payload + LWS_PRE, "%s", (char *)gateway_payload->payload);
        if (i <= 0)
        {
            ld_error("couldn't write payload to buffer");
            return -1;
        }
        lwsl_debug("TX: %s\n", payload + LWS_PRE);
        i = lws_write(wsi, (unsigned char *)(payload + LWS_PRE), strlen(gateway_payload->payload), LWS_WRITE_TEXT);
        if (i < 0)
        {
            lwsl_err("ERROR %d writing to socket, hanging up\n", i);
            return -1;
        }
        if (i < strlen(gateway_payload->payload))
        {
            lwsl_err("Partial write\n");
            return -1;
        }
        free(gateway_payload->payload);
        free(gateway_payload);

        break;
    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
        ld_info("lws: gateway initiated close of websocket: "
                "close code: %u\nCONTEXT (%d):\n%.*s",
                (unsigned int)((unsigned char *)in)[0] << 8 | ((unsigned char *)in)[1], len, len, in + 2);
        context->close_code = (unsigned int)((unsigned char *)in)[0] << 8 | ((unsigned char *)in)[1];

        if (len != 0)
        {
            close_message = strndup(in + 1, len);
        }

        i = context->user_callback(context, LD_CALLBACK_WS_GATEWAY_INIT_CLOSE, close_message, context->close_code);

        if (len != 0)
        {
            free(close_message);
        }

        if ((context->close_code == 1000) || (context->close_code == 4006))
        {

            context->gi[0]->session_valid = 0;
        }
        return 0;
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
    case LWS_CALLBACK_LOCK_POLL:
    case LWS_CALLBACK_UNLOCK_POLL:
        break;
    case LWS_CALLBACK_WS_CLIENT_DROP_PROTOCOL:
        ld_debug("%s: lws client dropped protocol", __FUNCTION__);
        break;
    case LWS_CALLBACK_CLIENT_CLOSED:
        ld_debug("%s: lws client closed", __FUNCTION__);

        break;
    case LWS_CALLBACK_WSI_DESTROY:
        ld_debug("%s: lws_wsi destroyed, clearing out", __FUNCTION__);
        context->lws_wsi = NULL;
    default:
        ld_debug("lws: received lws callback reason %d", reason);
        break;
    }
    return 0;
}

enum ld_gateway_payloadtype ld_gateway_payload_objectparser(const char *key)
{

    int cmp;
    cmp = strcmp(key, "op");
    if (cmp == 0)
        return LD_GATEWAY_OP;
    cmp = strcmp(key, "d");
    if (cmp == 0)
        return LD_GATEWAY_D;
    cmp = strcmp(key, "t");
    if (cmp == 0)
        return LD_GATEWAY_T;
    cmp = strcmp(key, "s");
    if (cmp == 0)
        return LD_GATEWAY_S;

    return LD_GATEWAY_UNKNOWN;
}
ld_status ld_set_identify(struct ld_context *context, struct ld_json_identify *identify)
{
    identify->properties = malloc(sizeof(struct ld_json_identify_connection_properties));
    if (identify->properties == NULL)
    {
        ld_error("%s: couldn't malloc identify properties", __FUNCTION__);
        return LDS_ALLOC_ERR;
    }
    identify->properties->device = context->device;
    identify->properties->os = context->os;
    identify->properties->browser = context->browser;

    identify->token = context->bot_token;
    identify->compress = 0;
    identify->large_threshold = 250;
    identify->shard[0] = 0;
    identify->shard[1] = context->shards;
    identify->status_update = context->init_presence;
    return LDS_OK;
}

ld_status ld_cleanup_identify(struct ld_json_identify *identify)
{
    if (identify->properties != NULL)
    {
        free(identify->properties);
    }

    return LDS_OK;
}

ld_status ld_gateway_payload_parser(struct ld_context *context, char *in, size_t len)
{

    json_t *payload, *value, *tmp;
    json_t *d, *t, *s, *op;
    json_error_t error;
    const char *key;
    payload = json_loadb(in, len, 0, &error);
    if (payload == NULL)
    {
        ld_debug("couldn't parse payload from gateway: %s", error.text);
        return 1;
    }
    d = NULL;
    enum ld_gateway_opcode opcode = LD_GATEWAY_OPCODE_UNKNOWN;

    json_object_foreach(payload, key, value)
    {

        switch (ld_gateway_payload_objectparser(key))
        {
        case LD_GATEWAY_OP:
            opcode = (enum ld_gateway_opcode)json_integer_value(value);
            ld_debug("received opcode %d", opcode);
            break;
        case LD_GATEWAY_D:
            d = value;

            break;
        case LD_GATEWAY_T:
            t = value;
            break;
        case LD_GATEWAY_S:
            context->last_seq = (int)json_integer_value(value);
            break;
        case LD_GATEWAY_UNKNOWN:
            ld_warning("%s: got unknown key in payload", __FUNCTION__);
            return 1;
        }
    }

    unsigned int hbi = 41250;
    switch (opcode)
    {
    case LD_GATEWAY_OPCODE_DISPATCH:

        return ld_gateway_dispatch_parser(context, t, d);
    case LD_GATEWAY_OPCODE_HEARTBEAT:

        break;
    case LD_GATEWAY_OPCODE_IDENTIFY:
        break;
    case LD_GATEWAY_OPCODE_PRESENCE:
        break;
    case LD_GATEWAY_OPCODE_VOICE_STATE:
        break;
    case LD_GATEWAY_OPCODE_VOICE_PING:
        break;
    case LD_GATEWAY_OPCODE_RESUME:
        break;
    case LD_GATEWAY_OPCODE_RECONNECT:
        break;
    case LD_GATEWAY_OPCODE_REQUEST_MEMBERS:
        break;
    case LD_GATEWAY_OPCODE_INVALIDATE_SESSION:

        free(context->gateway_session_id);
        context->gateway_session_id = NULL;
        context->last_seq = 0;
        break;
    case LD_GATEWAY_OPCODE_HELLO:

        if (d == NULL)
        {
            ld_warning("couldn't get d field in hello payload");
        }

        tmp = json_object_get(d, "heartbeat_interval");
        if (tmp != NULL)
        {
            if (json_integer_value(tmp) != 0)
            {
                hbi = (unsigned int)json_integer_value(tmp);
            }
            else
            {
                ld_warning("unexpected type for heartbeat interval in "
                           "hello payload (not integer)");
            }
        }
        else
        {
            ld_warning("couldn't find heartbeat interval in hello payload");
        }

        ld_debug("heartbeat interval is %d", hbi);
        context->heartbeat_interval = hbi;

        struct ld_json_identify ident;
        char *out;
        if (context->gateway_session_id != NULL)
        {

            ld_debug("%s: have previous gsid, will try resuming", __FUNCTION__);
            struct ld_json_resume resume;

            resume.session_id = context->gateway_session_id;
            resume.token = context->bot_token;
            resume.seq = context->last_seq;

            ld_json_save_resume(&out, &resume);
        }
        else
        {
            ld_debug("%s: no previous gsid, will try new connection identify", __FUNCTION__);
            ld_set_identify(context, &ident);
        }

        op = json_integer(LD_GATEWAY_OPCODE_IDENTIFY);
        t = NULL;
        s = NULL;

        d = ld_json_unpack_identify(&ident);
        ld_cleanup_identify(&ident);

        if (d == NULL)
        {
            ld_error("couldn't generate identify payload, closing gateway connection");
            return 1;
        }

        payload = ld_json_create_payload(op, d, t, s);

        struct ld_gateway_payload *toinsert;
        toinsert = malloc(sizeof(struct ld_gateway_payload));
        toinsert->payload = strdup(json_dumps(payload, 0));
        toinsert->len = strlen(toinsert->payload);
        lws_ring_insert(context->gateway_ring, toinsert, 1);

        json_decref(payload);
        ld_debug("prepared JSON identify payload: \n%s", (char *)toinsert->payload);

        context->last_hb = lws_now_secs();
        break;
    case LD_GATEWAY_OPCODE_HEARTBEAT_ACK:
        context->hb_count--;
        break;
    case LD_GATEWAY_OPCODE_GUILD_SYNC:

        break;
    case LD_GATEWAY_OPCODE_UNKNOWN:
    default:
        ld_warning("received payload with unknown opcode");
        return 1;
    }

    return 0;
}

ld_status ld_dispatch_ready(struct ld_context *context, json_t *data)
{

    const char *key;
    json_t *value;
    json_object_foreach(data, key, value)
    {
        if (strcmp(key, "session_id") == 0)
        {
            ld_debug("gateway session_id: %s", json_string_value(value));
            context->gateway_session_id = strdup(json_string_value(value));
            return LDS_OK;
        }
    }
    return LDS_ERROR;
}

ld_status ld_gateway_dispatch_parser(struct ld_context *context, json_t *type, json_t *data)
{

    int i, ret;
    const char *typestr;
    typestr = json_string_value(type);
    if (typestr == NULL)
    {
        ld_warning("jansson: couldn't identify gateway dispatch type");
        return LDS_JSON_GATEWAY_DISPATCH_TYPE_ERR;
    }

    struct ld_dispatch dispatch_dict[] = {
        {"READY", LD_CALLBACK_READY, (int (*)(struct ld_context *, json_t *)) & ld_dispatch_ready},
        {"CHANNEL_CREATE", LD_CALLBACK_CHANNEL_CREATE},
        {"CHANNEL_UPDATE", LD_CALLBACK_CHANNEL_UPDATE},
        {"CHANNEL_DELETE", LD_CALLBACK_CHANNEL_DELETE},
        {"CHANNEL_PINS_UPDATE", LD_CALLBACK_CHANNEL_PINS_UPDATE},
        {"GUILD_CREATE", LD_CALLBACK_GUILD_CREATE},
        {"GUILD_UPDATE", LD_CALLBACK_GUILD_UPDATE},
        {"GUILD_DELETE", LD_CALLBACK_GUILD_DELETE},
        {"GUILD_BAN_ADD", LD_CALLBACK_GUILD_BAN_ADD},
        {"GUILD_BAN_REMOVE", LD_CALLBACK_GUILD_BAN_REMOVE},
        {"GUILD_EMOJIS_UPDATE", LD_CALLBACK_GUILD_EMOJIS_UPDATE},
        {"GUILD_INTEGRATIONS_UPDATE", LD_CALLBACK_GUILD_INTEGRATIONS_UPDATE},
        {"GUILD_MEMBER_ADD", LD_CALLBACK_GUILD_MEMBER_ADD},
        {"GUILD_MEMBER_REMOVE", LD_CALLBACK_GUILD_MEMBER_REMOVE},
        {"GUILD_MEMBER_UPDATE", LD_CALLBACK_GUILD_MEMBER_UPDATE},
        {"GUILD_MEMBERS_CHUNK", LD_CALLBACK_GUILD_MEMBERS_CHUNK},
        {"GUILD_ROLE_CREATE", LD_CALLBACK_GUILD_ROLE_CREATE},
        {"GUILD_ROLE_UPDATE", LD_CALLBACK_GUILD_ROLE_UPDATE},
        {"GUILD_ROLE_DELETE", LD_CALLBACK_GUILD_ROLE_DELETE},
        {"MESSAGE_CREATE", LD_CALLBACK_MESSAGE_CREATE},
        {"MESSAGE_UPDATE", LD_CALLBACK_MESSAGE_UPDATE},
        {"MESSAGE_DELETE", LD_CALLBACK_MESSAGE_DELETE},
        {"MESSAGE_DELETE_BULK", LD_CALLBACK_MESSAGE_DELETE_BULK},
        {"MESSAGE_REACTION_ADD", LD_CALLBACK_MESSAGE_REACTION_ADD},
        {"MESSAGE_REACTION_REMOVE", LD_CALLBACK_MESSAGE_REACTION_REMOVE},
        {"MESSAGE_REACTION_REMOVE_ALL", LD_CALLBACK_MESSAGE_REACTION_REMOVE_ALL},
        {"PRESENCE_UPDATE", LD_CALLBACK_PRESENCE_UPDATE},
        {"TYPING_START", LD_CALLBACK_TYPING_START},
        {"USER_UPDATE", LD_CALLBACK_USER_UPDATE},
        {"VOICE_STATE_UPDATE", LD_CALLBACK_VOICE_STATE_UPDATE},
        {"VOICE_SERVER_UPDATE", LD_CALLBACK_VOICE_SERVER_UPDATE},
        {"WEBHOOKS_UPDATE", LD_CALLBACK_WEBHOOKS_UPDATE},
        {NULL, LD_CALLBACK_UNKNOWN}};

    ret = LDS_OK;

    for (i = 0; dispatch_dict[i].name != NULL; i++)
    {
        if (strcmp(typestr, dispatch_dict[i].name) == 0)
        {
            ld_debug("dispatch type is %s, callback reason is %d", dispatch_dict[i].name,
                     dispatch_dict[i].cbk_reason);
            if (dispatch_dict[i].dispatch_callback != NULL)
            {

                ret = dispatch_dict[i].dispatch_callback(context, data);
                if (ret != LDS_OK)
                {
                    ld_error("dispatch parser: internal dispatch callback errored out");
                    return LDS_ERROR;
                }
            }
            i = context->user_callback(context, dispatch_dict[i].cbk_reason, data, 0);
            return i;
        }
    }
    return LDS_OK;
}

ld_status ld_gateway_queue_heartbeat(struct ld_context *context)
{
    size_t ret;
    struct ld_gateway_payload *tmp;
    json_t *hb;

    hb = ld_json_create_payload(json_integer(LD_GATEWAY_OPCODE_HEARTBEAT),
                                json_integer(context->last_seq), NULL, NULL);

    if (lws_ring_get_count_free_elements(context->gateway_ring) == 0)
    {
        ld_warning("%s: can't fit any new payloads into gateway ringbuffer", __FUNCTION__);
        return LDS_WEBSOCKET_CANTFIT_PAYLOAD_ERR;
    }

    tmp = malloc(sizeof(struct ld_gateway_payload));
    tmp->len = strlen(json_dumps(hb, 0));
    tmp->payload = strdup(json_dumps(hb, 0));

    ret = lws_ring_insert(context->gateway_ring, tmp, 1);
    if (ret != 1)
    {
        ld_warning("couldn't fit heartbeat payload into gateway ringbuffer");
        return LDS_WEBSOCKET_CANTFIT_HEARTBEAT_ERR;
    }

    return LDS_OK;
}

char *ld_get_os_name()
{
#ifdef __APPLE__
    FILE *fd = popen("uname -s", "r");
    char *os_name, rxbuf[100], *ret;
    if (fd == NULL)
    {

        return strdup("unknown");
    }
    ret = fgets(rxbuf, 99, fd);
    if (ret != rxbuf)
    {

        return strdup("unknown");
    }

    pclose(fd);

    ld_debug("%s: os name: %s", __FUNCTION__, rxbuf);
    os_name = strdup(rxbuf);
    return os_name;
#else
    FILE *fd = popen("uname -o", "r");
    char *os_name, rxbuf[100], *ret;
    if (fd == NULL)
    {

        return strdup("unknown");
    }
    ret = fgets(rxbuf, 99, fd);
    if (ret != rxbuf)
    {

        return strdup("unknown");
    }

    pclose(fd);

    ld_debug("%s: os name: %s", __FUNCTION__, rxbuf);
    os_name = strdup(rxbuf);
    return os_name;
#endif
}
