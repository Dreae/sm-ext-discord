#include "DiscordClient.hpp"
#include "include/rust.h"

DiscordClient::DiscordClient(IdentityToken_t *plugin) {
    this->handler = create_handler(plugin);
}

void *DiscordClient::GetHandler() {
    return this->handler;
}