#include "glue.hpp"
#include "handletypes.hpp"

void call_message_callback(void *callback, IdentityToken_t *plugin, DiscordMessage *msg) {
    if (callback) {
        auto cb = (IPluginFunction *)callback;

        HandleError err;
        auto hndl = handlesys->CreateHandle(g_MessageType, msg, plugin, myself->GetIdentity(), &err);
        if (!hndl) {
            smutils->LogError(myself, "Got null handle, error code: %d", err);
        }

        cb->PushCell(hndl);
        cb->Execute(nullptr);

        handlesys->FreeHandle(hndl, nullptr);
    }
}

void log_error(char *msg) {
    smutils->LogError(myself, msg);
}

void log_message(char *msg) {
    smutils->LogMessage(myself, msg);
}