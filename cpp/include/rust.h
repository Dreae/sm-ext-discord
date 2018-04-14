#pragma once

typedef unsigned long long int u64_t;
typedef unsigned char bool_t;

extern "C" {
    typedef struct {
        char *content;
        u64_t channel_id;
        u64_t author_id;
        bool_t bot;
    } DiscordMessage;

    typedef struct {
        u64_t user_id;
    } DiscordReady;

    void *create_client(void *plugin, char *token);
    void client_set_msg_callback(void *client, void *callback);
    void client_set_ready_callback(void *client, void *callback);
    void connect_client(void *client);

    void say_to_channel(u64_t channel_id, char *content);
}