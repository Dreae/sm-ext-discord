#include "Extension.hpp"
#include "handletypes.hpp"
#include "include/rust.h"


HandleType_t g_UserType;
extern const sp_nativeinfo_t discord_user_natives[];

class DiscordUserNatives : public BaseClass, public IHandleTypeDispatch {
public:
    void OnExtLoad() {
        HandleAccess hacc;
        TypeAccess tacc;

        handlesys->InitAccessDefaults(&tacc, &hacc);
        tacc.ident = myself->GetIdentity();
        hacc.access[HandleAccess_Read] = HANDLE_RESTRICT_OWNER;

        g_UserType = handlesys->CreateType("DiscordUser", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, discord_user_natives);
    }

    void OnExtUnload() {
        handlesys->RemoveType(g_UserType, myself->GetIdentity());
    }

    void OnHandleDestroy(HandleType_t type, void *object) {
        free_discord_user(object);
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t type, void* object, unsigned int *pSize) {
        return false;
    }
};

DiscordUserNatives userNatives;

static cell_t native_GetName(IPluginContext *pContext, const cell_t *params) {
    DiscordUser *user = ReadHandle<DiscordUser>(pContext, params[1], g_UserType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);

    user_get_username(user, reinterpret_cast<char *>(addr), params[3]);

    return 1;
}

static cell_t native_GetTag(IPluginContext *pContext, const cell_t *params) {
    DiscordUser *user = ReadHandle<DiscordUser>(pContext, params[1], g_UserType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);

    user_get_tag(user, reinterpret_cast<char *>(addr), params[3]);

    return 1;
}

static cell_t native_HasRole(IPluginContext *pContext, const cell_t *params) {
    DiscordUser *user = ReadHandle<DiscordUser>(pContext, params[1], g_UserType);

    cell_t *guildAddr;
    pContext->LocalToPhysAddr(params[2], &guildAddr);

    cell_t *roleAddr;
    pContext->LocalToPhysAddr(params[3], &roleAddr);

    return user_has_role(user, *reinterpret_cast<u64_t *>(guildAddr), *reinterpret_cast<u64_t *>(roleAddr));
}

const sp_nativeinfo_t discord_user_natives[] = {
    "DiscordUser.GetName", native_GetName,
    "DiscordUser.GetTag", native_GetTag,
    "DiscordUser.HasRole", native_HasRole,
    {NULL, NULL}
};