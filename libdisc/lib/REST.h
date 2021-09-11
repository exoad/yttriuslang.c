/** @file */

#ifndef LIBDISCORD_REST_H
#define LIBDISCORD_REST_H

#include <curl/curl.h>

#include "status.h"
#include "json.h"

#ifndef LD_SNOWFLAKE
#define LD_SNOWFLAKE unsigned long long
#endif

struct ld_context;

struct ld_json_user;

enum ld_rest_http_verb
{
    LD_REST_VERB_GET,
    LD_REST_VERB_PUT,
    LD_REST_VERB_POST,
    LD_REST_VERB_PATCH,
    LD_REST_VERB_DELETE
};

struct ld_headers
{

    int length;
    char **key;
    char **value;
};

struct ld_rest_request
{
    enum ld_rest_http_verb verb;
    char *base_url;
    char *endpoint;
    char *body;
    size_t body_size;
    struct ld_headers *headers;
    int timeout;
    char *user_agent;
};

struct ld_rest_response
{
    long http_status;
    struct ld_headers *headers;
    char *body;
    size_t body_length;
};

struct ld_headers *ld_headers_init(struct ld_headers *headers);

int ld_headers_put(struct ld_headers *headers, char *key, char *value);

int ld_headers_clean(struct ld_headers *headers);

int ld_headers2curl(struct ld_headers *headers, struct curl_slist **slist);

struct ld_rest_request *ld_rest_init_request(struct ld_rest_request *request, struct ld_context *context);

struct ld_rest_response *ld_rest_init_response(struct ld_rest_response *response);

int ld_rest_free_request(struct ld_rest_request *request);

int ld_rest_free_response(struct ld_rest_response *response);

size_t ld_rest_writefunction(void *ptr, size_t size, size_t nmemb, struct ld_rest_response *response);

int ld_rest_send_request(struct ld_context *context, struct ld_rest_response *response, struct ld_rest_request *request);

int _ld_rest_blocking_request(
    enum ld_rest_http_verb verb,
    char *url,
    struct curl_slist headers,
    char *user_agent);

char *ld_rest_verb_enum2str(enum ld_rest_http_verb verb);

struct ld_rest_request *ld_get_gateway(struct ld_rest_request *req, struct ld_context *context);

struct ld_rest_request *ld_get_gateway_bot(struct ld_context *context, struct ld_rest_request *req);

int ld_create_basic_message(struct ld_context *context, struct ld_rest_request *req, LD_SNOWFLAKE channel_id,
                            const char *message_content);

int ld_send_basic_message(struct ld_context *context, LD_SNOWFLAKE channelid, const char *message);

int ld_get_current_user(struct ld_context *context, struct ld_json_user *user);

#endif
