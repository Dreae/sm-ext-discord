#include "Extension.hpp"
#include "include/rust.h"
#include "handletypes.hpp"
#include <cstdlib>

extern const sp_nativeinfo_t smdiscord_natives[];

class DiscordNatives : public BaseClass {
public:
    void OnExtLoad() {
        sharesys->AddNatives(myself, smdiscord_natives);
    }
};

DiscordNatives natives;

static cell_t native_UInt64ToString(IPluginContext *pContext, const cell_t *params) {
    cell_t *value;
    pContext->LocalToPhysAddr(params[1], &value);
    u64_t val = *reinterpret_cast<u64_t *>(value);

    char *buffer;
    pContext->LocalToString(params[2], &buffer);

    snprintf(buffer, 20, "%llu", val);

    return 1;
}

static cell_t native_StringToUInt64(IPluginContext *pContext, const cell_t *params) {
    char *buffer;
    pContext->LocalToString(params[1], &buffer);

    u64_t value = strtoull(buffer, nullptr, 10);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    *reinterpret_cast<u64_t *>(addr) = value;

    return 1;
}

static cell_t native_SendToChannel(IPluginContext *pContext, const cell_t *params) {
    auto new_message = ReadHandle<NewDiscordMessage>(pContext, params[2], g_NewMessageType);
    if (!new_message) {
        pContext->ReportError("Invalid message handle");
        return 0;
    }

    cell_t *addr;
    pContext->LocalToPhysAddr(params[1], &addr);
    u64_t channel_id = *reinterpret_cast<u64_t *>(addr);

    send_new_discord_message(channel_id, new_message);

    HandleSecurity sec;
    sec.pOwner = pContext->GetIdentity();
    sec.pIdentity = myself->GetIdentity();
    handlesys->FreeHandle(static_cast<Handle_t>(params[1]), &sec);

    return 1;
}

static cell_t native_FetchUser(IPluginContext *pContext, const cell_t *params) {
    cell_t *addr;
    pContext->LocalToPhysAddr(params[1], &addr);
    u64_t user_id = *reinterpret_cast<u64_t *>(addr);

    IPluginFunction *callback = pContext->GetFunctionById(params[2]);
    IdentityToken_t *ident = pContext->GetIdentity();

    get_user(user_id, callback, ident, params[3]);
}

const sp_nativeinfo_t smdiscord_natives[] = {
    {"UInt64ToString", native_UInt64ToString},
    {"StringToUInt64", native_StringToUInt64},
    {"Discord_SendToChannel", native_SendToChannel},
    {"Discord_FetchUser", native_FetchUser},
    {NULL, NULL}
};
