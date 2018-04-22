#include "glue.hpp"
#include "handletypes.hpp"

void call_message_callback(DiscordUser *author, DiscordMessage *msg) {
    HandleError err;
    auto hndl = handlesys->CreateHandle(g_MessageType, msg, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    auto author_hndl = handlesys->CreateHandle(g_UserType, author, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    g_MessageForward->PushCell(author_hndl);
    g_MessageForward->PushCell(hndl);
    g_MessageForward->Execute();

    handlesys->FreeHandle(hndl, nullptr);
    handlesys->FreeHandle(author_hndl, nullptr);
}

void call_ready_callback(DiscordReady *ready) {
    HandleError err;
    auto hndl = handlesys->CreateHandle(g_ReadyType, ready, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    g_ReadyForward->PushCell(hndl);
    g_ReadyForward->Execute();

    handlesys->FreeHandle(hndl, nullptr);
}

void call_user_callback(DiscordUser *user, IPluginFunction *callback, IdentityToken_t *plugin, i32_t data) {
    if (callback) {
        HandleError err;
        auto hndl = handlesys->CreateHandle(g_UserType, user, plugin, myself->GetIdentity(), &err);
        if (!hndl) {
            smutils->LogError(myself, "Got null handle, error code: %d", err);
        }

        callback->PushCell(hndl);
        callback->PushCell(data);
        callback->Execute(nullptr);
    }
}

void log_error(char *msg) {
    smutils->LogError(myself, msg);
}

void log_message(char *msg) {
    smutils->LogMessage(myself, msg);
}