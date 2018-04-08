#include "Extension.hpp"
#include "include/rust.h"

extern "C" void call_message_callback(void *callback, DiscordMessage *msg);