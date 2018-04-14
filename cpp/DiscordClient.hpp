#pragma once

#include "Extension.hpp"

class DiscordClient {
    friend class ClientNatives;
private:
    void *handler;
public:
    DiscordClient(IdentityToken_t *plugin);
    void *GetHandler();
    void SetMessageCallback(IPluginFunction *callback);
    void Connect(char *token);
};