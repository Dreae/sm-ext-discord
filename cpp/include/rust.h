#pragma once

extern "C" {
    typedef struct {
        char *content;
    } DiscordMessage;

    void *create_handler();
    void handler_set_msg_callback(void *handler, void *callback);
    void connect_handler(void *handler, char *token);
    void free_discord_message(DiscordMessage *msg);
}