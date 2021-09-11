/** @file */

#ifndef LIBDISCORD_JSON_H
#define LIBDISCORD_JSON_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <libdiscord.h>
#include <jansson.h>
#include <time.h>

#include "REST.h"

enum ld_gateway_opcode;
enum ld_dispatch_event;

enum ld_presence_activity_type;
enum ld_json_status_type;
enum ld_activity_flags;

struct ld_timestamp;

struct ld_json_snowflake;
struct ld_json_status_update;
struct ld_json_identify_connection_properties;
struct ld_json_identify;
struct ld_json_party;
struct ld_json_assets;
struct ld_json_activity;
struct ld_json_secrets;
struct ld_json_gateway_update_status;
struct ld_json_user;
struct ld_json_role;
struct ld_json_attachemnt;
struct ld_json_embed_footer;
struct ld_json_embed_image;
struct ld_json_embed_thumbnail;
struct ld_json_embed_video;
struct ld_json_embed_provider;
struct ld_json_embed_author;
struct ld_json_embed_field;
struct ld_json_embed;
struct ld_json_emoji;
struct ld_json_reaction;
struct ld_json_message_activity;
struct ld_json_message_application;
struct ld_json_message;
struct ld_json_overwrite;
struct ld_json_attachment;
struct ld_json_channel;
struct ld_json_voice_state;
struct ld_json_guild_member;
struct ld_json_guild;
struct ld_json_account;
struct ld_json_ban;
struct ld_json_integration;

struct ld_json_getgateway;
struct ld_json_getgateway_bot;
struct ld_json_getgateway_bot_sessionstartlimit;

struct ld_json_websocket_payload;
struct ld_json_resume;

#pragma mark Enumeration declarations

/**
 * @brief activity type
 *
 * <a href="https:
 */
enum ld_presence_activity_type
{
    LD_PRESENCE_ACTIVITY_PLAYING = 0,
    LD_PRESENCE_ACTIVITY_STREAMING = 1,
    LD_PRESENCE_ACTIVITY_LISTENING = 2

};

/**
 * @brief Status enum type
 *
 * enum integer values are used internally, strings are sent in payloads
 *
 * <a href="https:
 */
enum ld_json_status_type
{
    LD_PRESENCE_IDLE = 0,
    LD_PRESENCE_DND = 1,
    LD_PRESENCE_ONLINE = 2,
    LD_PRESENCE_OFFLINE = 3,
    LD_PRESENCE_INVISIBLE = 4
};

/**
 * @brief Activity flags enum type
 *
 * enum values are meant to be OR'd together
 *
 * <a href="https:
 */
enum ld_activity_flags
{
    LD_ACTIVITY_FLAG_INSTANCE = 1 << 0,
    LD_ACTIVITY_FLAG_JOIN = 1 << 1,
    LD_ACTIVITY_FLAG_SPECTATE = 1 << 2,
    LD_ACTIVITY_FLAG_JOIN_REQUEST = 1 << 3,
    LD_ACTIVITY_FLAG_SYNC = 1 << 4,
    LD_ACTIVITY_FLAG_PLAY = 1 << 5

};

#pragma mark Struct declarations

/**
 * @brief timestamp struct
 * used for timestamp fields in events
 * example string: 2019-02-09T21:31:47.083000+00:00
 */
struct ld_timestamp
{
    char *iso_str;
    uint64_t unix_epoch;
    uint64_t discord_epoch;
};

/**
 * @brief decoded snowflake struct
 *
 * <a href="https:
 */
struct ld_json_snowflake
{
    uint64_t timestamp;
    unsigned long long worker_id;
    unsigned long long process_id;
    unsigned long long increment;

    uint64_t unix_timestamp;
};

/**
 * @brief Status update
 *
 * <a href="https:
 */
struct ld_json_status_update
{

    uint64_t *roles;
    struct ld_json_activity *game;
    uint64_t guild_id;
    enum ld_json_status_type status;
    int since;
    int afk;
};

/**
 * @brief Identify connection properties json object
 *
 * <a href="https:
 */
struct ld_json_identify_connection_properties
{
    char *os;
    char *browser;
    char *device;
};

/**
 * @brief Identify json object
 * sent with opcode 2 payloads (IDENTIFY)
 *
 * <a href="https:
 *
 */
struct ld_json_identify
{
    char *token;
    struct ld_json_identify_connection_properties *properties;
    int compress;
    int large_threshold;
    int shard[2];
    struct ld_json_status_update *status_update;
};

/**
 * @brief Party json object
 *
 * <a href="https:
 *
 */
struct ld_json_party
{
    char *id;
    int size[2];
};

/**
 * @brief Activity assets json object
 *
 * <a href="https:
 *
 */
struct ld_json_assets
{
    char *large_image;
    char *large_text;
    char *small_image;
    char *small_text;
};

/**
 * @brief Activity json object
 * bots may only send name, type, and optionally url
 *
 * <a href="https:
 *
 */
struct ld_json_activity
{
    char *name;
    enum ld_presence_activity_type type;
    char *url;
    struct ld_json_timestamps *timestamps;
    uint64_t application_id;
    char *details;
    char *state;
    struct ld_json_party *party;
    struct ld_json_assets *assets;
    struct ld_json_secrets *secrets;
    int flags;
};

/**
 * @brief Activity secrets json object
 *
 * <a href="https:
 *
 */
struct ld_json_secrets
{
    char *join;
    char *spectate;
    char *match;
};

/**
 * @brief Gateway status update json object
 *
 * <a href="https:
 */

struct ld_json_gateway_update_status
{
    int since;
    struct ld_json_activity *game;
    char *status;
    int afk;
};

/**
 *
 */
struct ld_json_user
{
    uint64_t id;
    char *username;
    char *discriminator;
    char *avatar;
    int bot;
    int mfa_enabled;
    char *locale;
    int verified;
    char *email;
};

struct ld_json_role
{
    uint64_t id;
    char *name;
    int color;
    int hoist;
    int position;
    int permissions;
    int managed;
    int mentionable;
};

struct ld_json_attachemnt
{
    uint64_t id;
    char *filename;
    int size;
    char *url;
    char *proxy_url;
    int height;
    int width;
};

struct ld_json_embed_footer
{
    char *text;
    char *icon_url;
    char *proxy_icon_url;
};

struct ld_json_embed_image
{
    char *url;
    char *proxy_url;
    int height;
    int width;
};

struct ld_json_embed_thumbnail
{
    char *url;
    char *proxy_url;
    int height;
    int width;
};

struct ld_json_embed_video
{
    char *url;
    int height;
    int width;
};

struct ld_json_embed_provider
{
    char *name;
    char *url;
};

struct ld_json_embed_author
{
    char *name;
    char *url;
    char *icon_url;
    char *proxy_icon_url;
};

struct ld_json_embed_field
{
    char *name;
    char *value;
    int _inline;
};

struct ld_json_embed
{
    char *title;
    char *type;
    char *description;
    char *url;
    char *timestamp;
    int color;
    struct ld_json_embed_footer *footer;
    struct ld_json_embed_image *image;
    struct ld_json_embed_thumbnail *thumbnail;
    struct ld_json_embed_video *video;
    struct ld_json_embed_provider *provider;
    struct ld_json_embed_author *author;
    struct ld_json_embed_field *fields[];
};

struct ld_json_emoji
{
    uint64_t id;
    char *name;
    struct ld_json_role **roles;
    struct ld_json_user *user;
    int require_colons;
    int managed;
    int animated;
};

struct ld_json_reaction
{
    int count;
    int me;
    struct ld_json_emoji *emoji;
};

struct ld_json_message_activity
{
    int type;
    char *party_id;
};

struct ld_json_message_application
{
    uint64_t id;
    char *cover_image;
    char *description;
    char *icon;
    char *name;
};

struct ld_json_message
{
    uint64_t id;
    uint64_t channel_id;
    struct ld_json_user *author;
    char *content;
    char *timestamp;
    char *edited_timestamp;
    int tts;
    int mention_everyone;
    struct ld_json_user **mentions;
    struct ld_json_role **mention_roles;
    struct ld_json_attachemnt **attachments;
    struct ld_json_embed **embeds;
    struct ld_json_reaction **reactions;
    uint64_t webhook_id;
    int type;
    struct ld_json_message_activity *activity;
    struct ld_json_message_application *application;
};

struct ld_json_overwrite
{
    uint64_t id;
    char *type;
    int allow;
    int deny;
};

struct ld_json_attachment
{
    uint64_t id;
    char *filename;
    int size;
    char *url;
    char *proxy_url;
    int height;
    int width;
};

struct ld_json_channel
{
    uint64_t id;
    int type;
    uint64_t guild_id;
    int position;
    struct ld_json_overwrite **permission_overwrites;
    char *name;
    char *topic;
    int nsfw;
    uint64_t last_message_id;
    int bitrate;
    int user_limit;
    struct ld_json_user **recipients;
    char *icon;
    uint64_t owner_id;
    uint64_t application_id;
    uint64_t parent_id;
    char *last_pin_timestamp;
};

struct ld_json_voice_state
{
    uint64_t guild_id;
    uint64_t channel_id;
    uint64_t user_id;
    char *session_id;
    int deaf;
    int mute;
    int self_deaf;
    int self_mute;
    int supress;
};

struct ld_json_guild_member
{
    struct ld_json_user *user;
    char *nick;
    uint64_t *roles;
    char *joined_at;
    int deaf;
    int mute;
};

struct ld_json_guild
{
    uint64_t id;
    char *name;
    char *icon;
    char *splash;
    int owner;
    uint64_t owner_id;
    int permissions;
    char *region;
    uint64_t afk_channel_id;
    int afk_timeout;
    int embed_enabled;
    uint64_t embed_channel_id;
    int verification_level;
    int default_message_notifications;
    int explicit_content_filter;
    struct ld_json_role **roles;
    struct ld_json_emoji **emojis;
    char **features;
    int mfa_level;
    uint64_t application_id;
    int widget_enabled;
    uint64_t widget_channel_id;
    uint64_t system_channel_id;
    char *joined_at;
    int large;
    int unavailable;
    int member_count;
    struct ld_json_voice_state **voice_states;
    struct ld_json_guild_member **members;
    struct ld_json_channel **channels;
    struct ld_json_status_update **presences;
};

struct ld_json_account
{
    uint64_t id;
    char *name;
};

struct ld_json_ban
{
    char *reason;
    struct ld_json_user *user;
};

struct ld_json_integration
{
    uint64_t id;
    char *name;
    char *type;
    int enabled;
    int syncing;
    uint64_t role_id;
    int expire_behavior;
    int expire_grace_period;
    struct ld_json_user *user;
    struct ld_json_account *account;
    char *synced_at;
};

struct ld_json_getgateway
{
    char *url;
};

struct ld_json_getgateway_bot
{
    char *url;
    int shards;
    struct ld_json_getgateway_bot_sessionstartlimit *limits;
};

struct ld_json_getgateway_bot_sessionstartlimit
{
    int total;
    int remaining;
    int reset_after;
};

/**
 * @brief Gateway payload
 *
 * @todo how to identify struct type?
 *
 * <a href="https:
 */
struct ld_json_websocket_payload
{
    enum ld_gateway_opcode op;
    void *d;
    int s;

    enum ld_dispatch_event t;
};

/**
 * @brief Gateway resume data field
 *
 * <a href="https:
 */
struct ld_json_resume
{
    char *token;
    char *session_id;
    int seq;
};

/**
 * @brief struct equivalent of "[]"
 * is this a good solution?
 */
struct ld_json_empty_array
{
};

json_t *ld_json_create_payload(json_t *op, json_t *d, json_t *t, json_t *s);

json_t *ld_json_create_message();

uint64_t ld_str2snowflake();
char *ld_snowflake2str(uint64_t flake);
json_t *ld_json_dump_activity(struct ld_json_activity *activity);
int ld_json_load_user(struct ld_json_user *new_user, json_t *user);
json_t *ld_json_unpack_user(struct ld_json_user *user);
json_t *ld_json_unpack_status_update(struct ld_json_status_update *status_update);
json_t *ld_json_unpack_identify_connection_properties(struct ld_json_identify_connection_properties *properties);
json_t *ld_json_unpack_identify(struct ld_json_identify *identify);
const char *ld_json_status2str(enum ld_json_status_type type);
int ld_json_message_init(struct ld_json_message *message);
int ld_json_message_cleanup(struct ld_json_message *message);
int *ld_json_pack_message(struct ld_json_message *new_message, json_t *message);
int ld_json_pack_snowflake(struct ld_json_snowflake *new_flake, uint64_t snowflake);
int ld_json_read_timestamp(struct ld_timestamp *new_timestamp, char *timestamp);

/**
 * @brief Internal json dump function used for most ld_json_dump_* functions
 * @param out Pointer to save string to, must be freed by caller
 * @param in json_t containing json data
 * @return LDS enum, LDS_OK on success, LDS_JSON_* on error
 */
ld_status _ld_json_dump_all(char **out, json_t *in, const char *caller);

ld_status ld_json_unpack_resume(json_t *out, struct ld_json_resume *resume);
ld_status ld_json_dump_resume(char **out, json_t *resume);
ld_status ld_json_save_resume(char **out, struct ld_json_resume *resume);

/**
 * @brief Checks payload struct against Discord's field rules
 * @param payload pointer to filled payload. Cannot be null.
 * @return LDS_OK if valid, LD_JSON_* error otherwise
 */
ld_status ld_json_payload_valid(struct ld_json_websocket_payload *payload);
ld_status ld_json_unpack_payload(json_t *out, struct ld_json_websocket_payload *payload);
ld_status ld_json_dump_payload(char **out, json_t *payload);
ld_status ld_json_save_payload(char **out, const struct ld_json_websocket_payload *payload);

#endif
