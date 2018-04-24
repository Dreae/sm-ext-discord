#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include "Extension.hpp"
#include "include/rust.h"

class CallbackItem {
public:
    virtual void Execute() = 0;
};

class MessageCallback : public CallbackItem {
private:
    DiscordMessage *message;
    DiscordUser *user;
public:
    MessageCallback(DiscordUser *user, DiscordMessage *message) : message(message), user(user) { };
    void Execute();
};

class ReadyCallback : public CallbackItem {
private:
    DiscordReady *ready;
public:
    ReadyCallback(DiscordReady *ready) : ready(ready) { };
    void Execute();
};

class UserCallback : public CallbackItem {
private:
    DiscordUser *user;
    IPluginFunction *callback;
    IdentityToken_t *plugin;
    cell_t data;
public:
    UserCallback(DiscordUser *user, IPluginFunction *callback, IdentityToken_t *plugin, cell_t data)
        : user(user)
        , callback(callback)
        , plugin(plugin)
        , data(data)
        { };
    void Execute();
};

extern std::mutex g_callback_mutex;
extern std::queue<std::unique_ptr<CallbackItem>> callback_queue;
extern void On_GameFrame(bool simulating);
extern void AddCallback(std::unique_ptr<CallbackItem> item);
