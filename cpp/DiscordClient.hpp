#pragma once

#include "Extension.hpp"

class DiscordClient {
    friend class ClientNatives;
private:
    void *handler;
public:
    DiscordClient();
    void SetMessageCallback(IPluginFunction *callback);
    void Connect(char *token);
};