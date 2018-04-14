#include "Extension.hpp"
#include "include/rust.h"

extern "C" void call_message_callback(void *callback, IdentityToken_t *plugin, DiscordMessage *msg);
extern "C" void log_error(char *msg);
extern "C" void log_message(char *msg);