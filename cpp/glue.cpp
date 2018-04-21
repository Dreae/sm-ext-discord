#include "glue.hpp"
#include "handletypes.hpp"

void call_message_callback(DiscordMessage *msg) {
    HandleError err;
    auto hndl = handlesys->CreateHandle(g_MessageType, msg, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    g_MessageForward->PushCell(hndl);
    g_MessageForward->Execute();
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
}

void log_error(char *msg) {
    smutils->LogError(myself, msg);
}

void log_message(char *msg) {
    smutils->LogMessage(myself, msg);
}