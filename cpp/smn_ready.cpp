#include "Extension.hpp"
#include "handletypes.hpp"
#include "include/rust.h"


HandleType_t g_ReadyType;
extern const sp_nativeinfo_t discord_ready_natives[];

class DiscordReadyNatives : public BaseClass, public IHandleTypeDispatch {
public:
    void OnExtLoad() {
        HandleAccess hacc;
        TypeAccess tacc;

        handlesys->InitAccessDefaults(&tacc, &hacc);
        tacc.ident = myself->GetIdentity();
        hacc.access[HandleAccess_Read] = HANDLE_RESTRICT_OWNER;

        g_ReadyType = handlesys->CreateType("DiscordReady", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, discord_ready_natives);
    }

    void OnExtUnload() {
        handlesys->RemoveType(g_ReadyType, myself->GetIdentity());
    }

    void OnHandleDestroy(HandleType_t type, void *object) {
        return;
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t type, void* object, unsigned int *pSize) {
        return false;
    }
};

DiscordReadyNatives readyNatives;

static cell_t native_CurrentUserId(IPluginContext *pContext, const cell_t *params) {
    DiscordReady *ready = ReadHandle<DiscordReady>(pContext, params[1], g_ReadyType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    *reinterpret_cast<u64_t *>(addr) = ready->user_id;

    return 1;
}

const sp_nativeinfo_t discord_ready_natives[] = {
    {"DiscordReady.CurrentUserId", native_CurrentUserId},
    {NULL, NULL}
};