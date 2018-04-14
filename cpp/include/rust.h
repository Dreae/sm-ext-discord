#pragma once

typedef unsigned long long int u64_t;

extern "C" {
    typedef struct {
        char *content;
        u64_t channel_id;
    } DiscordMessage;

    void *create_handler(void *plugin);
    void handler_set_msg_callback(void *handler, void *callback);
    void connect_handler(void *handler, char *token);

    void say_to_channel(u64_t channel_id, char *content);
}