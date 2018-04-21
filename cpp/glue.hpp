#include "Extension.hpp"
#include "include/rust.h"

extern "C" void call_message_callback(DiscordMessage *msg);
extern "C" void call_ready_callback(DiscordReady *ready);
extern "C" void log_error(char *msg);
extern "C" void log_message(char *msg);