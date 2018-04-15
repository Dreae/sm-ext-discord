#pragma once

#include "Extension.hpp"

class DiscordClient {
    friend class ClientNatives;
private:
    RustDiscordClient *client;
public:
    DiscordClient(IdentityToken_t *plugin, char *token);
    RustDiscordClient *GetClient();
    void SetMessageCallback(IPluginFunction *callback);
    void Connect();
};