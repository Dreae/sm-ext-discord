#include "Extension.hpp"
#include "handletypes.hpp"
#include "DiscordClient.hpp"
#include "include/rust.h"

HandleType_t g_ClientType;
extern const sp_nativeinfo_t discord_client_natives[];

class ClientNatives : public BaseClass, public IHandleTypeDispatch {
public:
    void OnExtLoad() {
        HandleAccess hacc;
        TypeAccess tacc;

        handlesys->InitAccessDefaults(&tacc, &hacc);
        tacc.ident = myself->GetIdentity();
        hacc.access[HandleAccess_Read] = HANDLE_RESTRICT_OWNER;
        tacc.access[HTypeAccess_Create] = true;
        tacc.access[HTypeAccess_Inherit] = true;

        g_ClientType = handlesys->CreateType("DiscordClient", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, discord_client_natives);
    }

    void OnExtUnload() {
        handlesys->RemoveType(g_ClientType, myself->GetIdentity());
    }

    void OnHandleDestroy(HandleType_t type, void *object) {
        // TODO: Shutdown shards
        return;
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t, void* object, unsigned int *pSize) {
        return false;
    }
};

ClientNatives clientNatives;

static cell_t native_CreateClient(IPluginContext *pContext, const cell_t *params) {
    char *token;
    pContext->LocalToString(params[1], &token);

    auto client = new DiscordClient(pContext->GetIdentity(), token);
    auto hndl = handlesys->CreateHandle(g_ClientType, client, pContext->GetIdentity(), myself->GetIdentity(), NULL);

    return hndl;
}

static cell_t native_SetMessageCallback(IPluginContext *pContext, const cell_t *params) {
    DiscordClient *client = ReadHandle<DiscordClient>(pContext, params[1], g_ClientType);

    auto callback = pContext->GetFunctionById((funcid_t)params[2]);
    if (!callback) {
        pContext->ReportError("Invalid message callback");
    }

    client_set_msg_callback(client->GetClient(), callback);

    return 1;
}

static cell_t native_SetReadyCallback(IPluginContext *pContext, const cell_t *params) {
    DiscordClient *client = ReadHandle<DiscordClient>(pContext, params[1], g_ClientType);

    auto callback = pContext->GetFunctionById((funcid_t)params[2]);
    if (!callback) {
        pContext->ReportError("Invalid ready callback");
    }

    client_set_ready_callback(client->GetClient(), callback);

    return 1;
}

static cell_t native_ClientConnect(IPluginContext *pContext, const cell_t *params) {
    DiscordClient *client = ReadHandle<DiscordClient>(pContext, params[1], g_ClientType);

    connect_client(client->GetClient());

    return 1;
}

const sp_nativeinfo_t discord_client_natives[] = {
    {"DiscordClient.DiscordClient", native_CreateClient},
    {"DiscordClient.SetMessageCallback", native_SetMessageCallback},
    {"DiscordClient.SetReadyCallback", native_SetReadyCallback},
    {"DiscordClient.Connect", native_ClientConnect},
    {NULL, NULL}
};