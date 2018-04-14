#pragma once

typedef unsigned long long int u64_t;

extern "C" {
    typedef struct {
        char *content;
        u64_t channel_id;
    } DiscordMessage;

    void *create_client(void *plugin, char *token);
    void client_set_msg_callback(void *client, void *callback);
    void connect_client(void *client);

    void say_to_channel(u64_t channel_id, char *content);
}