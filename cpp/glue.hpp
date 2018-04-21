#include "Extension.hpp"
#include "include/rust.h"

extern "C" void call_message_callback(DiscordMessage *msg);
extern "C" void call_ready_callback(DiscordReady *ready);
extern "C" void call_user_callback(DiscordUser *user, IPluginFunction *callback, IdentityToken_t *plugin, i32_t data);
extern "C" void log_error(char *msg);
extern "C" void log_message(char *msg);