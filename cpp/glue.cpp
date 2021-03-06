#include "glue.hpp"
#include "handletypes.hpp"
#include "work_queue.hpp"

void call_message_callback(DiscordUser *author, DiscordMessage *msg) {
    AddCallback(std::unique_ptr<CallbackItem>(new MessageCallback(author, msg)));
}

void call_ready_callback(DiscordReady *ready) {
    AddCallback(std::unique_ptr<CallbackItem>(new ReadyCallback(ready)));
}

void call_user_callback(DiscordUser *user, IPluginFunction *callback, IdentityToken_t *plugin, i32_t data) {
    AddCallback(std::unique_ptr<CallbackItem>(new UserCallback(user, callback, plugin, data)));
}

void log_error(char *msg) {
    smutils->LogError(myself, msg);
}

void log_message(char *msg) {
    smutils->LogMessage(myself, msg);
}