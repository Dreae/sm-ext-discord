#pragma once

#include "Extension.hpp"

class DiscordClient {
    friend class ClientNatives;
private:
    void *client;
public:
    DiscordClient(IdentityToken_t *plugin, char *token);
    void *GetClient();
    void SetMessageCallback(IPluginFunction *callback);
    void Connect();
};