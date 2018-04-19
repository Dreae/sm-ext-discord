#pragma once

typedef unsigned long long int u64_t;
typedef unsigned long int u32_t;
typedef unsigned char bool_t;

typedef void RustDiscordClient;
typedef void NewDiscordMessage;
typedef void NewDiscordEmbed;

extern "C" {
    typedef struct {
        char *content;
        u64_t channel_id;
        u64_t author_id;
        bool_t bot;
        bool_t own;
        u64_t *mentioned_roles;
        u32_t num_mentioned_roles;
        u64_t *mentioned_users;
        u32_t num_mentioned_users;
    } DiscordMessage;

    typedef struct {
        u64_t user_id;
    } DiscordReady;

    RustDiscordClient *create_client(void *plugin, char *token);
    void client_set_msg_callback(RustDiscordClient *client, void *callback);
    void client_set_ready_callback(RustDiscordClient *client, void *callback);
    void connect_client(RustDiscordClient *client);

    void say_to_channel(u64_t channel_id, char *content);

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
}