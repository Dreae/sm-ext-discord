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
        return;
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t, void* object, unsigned int *pSize) {
        return false;
    }
};

ClientNatives clientNatives;

static cell_t native_CreateClient(IPluginContext *pContext, const cell_t *params) {
    auto client = new DiscordClient(pContext->GetIdentity());
    auto hndl = handlesys->CreateHandle(g_ClientType, client, pContext->GetIdentity(), myself->GetIdentity(), NULL);

    return hndl;
}

static cell_t native_SetMessageCallback(IPluginContext *pContext, const cell_t *params) {
    DiscordClient *client = ReadHandle<DiscordClient>(pContext, params, g_ClientType);

    auto callback = pContext->GetFunctionById((funcid_t)params[2]);
    if (!callback) {
        pContext->ReportError("Invalid message callback");
    }

    handler_set_msg_callback(client->GetHandler(), callback);

    return 0;
}

static cell_t native_ClientConnect(IPluginContext *pContext, const cell_t *params) {
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleSecurity sec;
    DiscordClient *client;

    sec.pOwner = pContext->GetIdentity();
    sec.pIdentity = myself->GetIdentity();
    auto herr = handlesys->ReadHandle(hndl, g_ClientType, &sec, reinterpret_cast<void **>(&client));

    if (herr != HandleError_None) {
        pContext->ReportError("Invalid Client handle %x (error %d)", hndl, herr);
        return 0;
    }

    char *token;
    pContext->LocalToString(params[2], &token);

    connect_handler(client->GetHandler(), token);

    handlesys->FreeHandle(hndl, &sec);

    return 1;
}

const sp_nativeinfo_t discord_client_natives[] = {
    {"DiscordClient.DiscordClient", native_CreateClient},
    {"DiscordClient.SetMessageCallback", native_SetMessageCallback},
    {"DiscordClient.Connect", native_ClientConnect},
    {NULL, NULL}
};