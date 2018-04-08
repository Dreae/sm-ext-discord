#include "DiscordClient.hpp"
#include "include/rust.h"

DiscordClient::DiscordClient() {
    this->handler = create_handler();
}