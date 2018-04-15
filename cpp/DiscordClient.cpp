#include "DiscordClient.hpp"
#include "include/rust.h"

DiscordClient::DiscordClient(IdentityToken_t *plugin, char *token) {
    this->client = create_client(plugin, token);
}

RustDiscordClient *DiscordClient::GetClient() {
    return this->client;
}