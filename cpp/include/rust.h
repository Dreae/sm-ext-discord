#pragma once

typedef unsigned long long int u64_t;
typedef unsigned long int u32_t;
typedef long int i32_t;
typedef unsigned char bool_t;

typedef void NewDiscordMessage;
typedef void NewDiscordEmbed;
typedef void DiscordUser;
typedef void DiscordReady;
typedef void DiscordMessage;

extern "C" {
    void start_discord_client(const char *token);

    void say_to_channel(u64_t channel_id, char *content);

    void free_discord_message(DiscordMessage *msg);
    void free_discord_user(DiscordUser *user);
    void free_discord_ready(DiscordReady *ready);

    void get_message_content(DiscordMessage *msg, char* buffer, size_t len);
    u64_t get_message_channel_id(DiscordMessage *msg);
    u64_t get_message_author_id(DiscordMessage *msg);
    u64_t get_message_guild_id(DiscordMessage *msg);
    bool_t get_message_is_self(DiscordMessage *msg);
    bool_t get_message_is_bot(DiscordMessage *msg);
    bool_t get_message_mentions_role(DiscordMessage *msg, u64_t role_id);
    u32_t get_message_num_mentioned_roles(DiscordMessage *msg);
    u64_t get_message_mentioned_role(DiscordMessage *msg, u32_t which);
    bool_t get_message_mentions_user(DiscordMessage *msg, u64_t role_id);
    u32_t get_message_num_mentioned_users(DiscordMessage *msg);
    u64_t get_message_mentioned_user(DiscordMessage *msg, u32_t which);

    u64_t get_ready_user_id(DiscordReady *ready);

    NewDiscordMessage *create_new_discord_message();
    void set_new_message_content(NewDiscordMessage *new_message, char *content);
    void set_new_message_embed(NewDiscordMessage *new_message, NewDiscordEmbed *embed);
    void send_new_discord_message(u64_t channel_id, NewDiscordMessage *new_message);

    NewDiscordEmbed *create_new_discord_embed();
    void set_new_embed_title(NewDiscordEmbed *new_embed, char *content);
    void set_new_embed_description(NewDiscordEmbed *new_embed, char *description);
    void new_embed_add_field(NewDiscordEmbed *new_embed, char *title, char *value, bool_t in_line);
    void new_embed_set_color(NewDiscordEmbed *new_embed, unsigned char r, unsigned char g, unsigned char b);
    void new_embed_set_footer_text(NewDiscordEmbed *new_embed, char *text);
    void new_embed_set_footer_icon(NewDiscordEmbed *new_embed, char *url);

    DiscordUser *get_user(u64_t user_id, void *callback, void *plugin, i32_t data);
    void user_get_username(DiscordUser *user, char *buffer, size_t len);
    void user_get_tag(DiscordUser *user, char *buffer, size_t len);
    bool_t user_has_role(DiscordUser *user, u64_t guild_id, u64_t role_id);
}