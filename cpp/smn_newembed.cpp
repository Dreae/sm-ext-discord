#include "Extension.hpp"
#include "handletypes.hpp"
#include "include/rust.h"

HandleType_t g_NewEmbedType;
extern const sp_nativeinfo_t newembed_natives[];

class NewDiscordEmbedNatives : public BaseClass, public IHandleTypeDispatch {
public:
    void OnExtLoad() {
        HandleAccess hacc;
        TypeAccess tacc;

        handlesys->InitAccessDefaults(&tacc, &hacc);
        tacc.ident = myself->GetIdentity();
        hacc.access[HandleAccess_Read] = HANDLE_RESTRICT_OWNER;

        g_NewEmbedType = handlesys->CreateType("NewDiscordEmbed", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, newembed_natives);
    }
    void OnExtUnload() {
        handlesys->RemoveType(g_NewEmbedType, myself->GetIdentity());
    }

    void OnHandleDestroy(HandleType_t type, void *object) {
        return;
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t type, void* object, unsigned int *pSize) {
        return false;
    }
};

NewDiscordEmbedNatives new_embed_natives;

static cell_t native_CreateNewDiscordEmbed(IPluginContext *pContext, const cell_t *params) {
    auto new_embed = create_new_discord_embed();
    auto hndl = handlesys->CreateHandle(g_NewEmbedType, new_embed, pContext->GetIdentity(), myself->GetIdentity(), NULL);

    return hndl;
}

static cell_t native_SetNewEmbedTitle(IPluginContext *pContext, const cell_t *params) {
    auto new_embed = ReadHandle<NewDiscordEmbed>(pContext, params[1], g_NewEmbedType);

    char *title;
    pContext->LocalToString(params[2], &title);

    set_new_embed_title(new_embed, title);

    return 1;
}

static cell_t native_SetNewEmbedDescription(IPluginContext *pContext, const cell_t *params) {
    auto new_embed = ReadHandle<NewDiscordEmbed>(pContext, params[1], g_NewEmbedType);

    char *description;
    pContext->LocalToString(params[2], &description);

    set_new_embed_description(new_embed, description);

    return 1;
}

static cell_t native_NewEmbedAddField(IPluginContext *pContext, const cell_t *params) {
    auto new_embed = ReadHandle<NewDiscordEmbed>(pContext, params[1], g_NewEmbedType);

    char *title;
    char *value;
    pContext->LocalToString(params[2], &title);
    pContext->LocalToString(params[3], &value);

    new_embed_add_field(new_embed, title, value, params[4]);
}

const sp_nativeinfo_t newembed_natives[] = {
    {"NewDiscordEmbed.NewDiscordEmbed", native_CreateNewDiscordEmbed},
    {"NewDiscordEmbed.SetTitle", native_SetNewEmbedTitle},
    {"NewDiscordEmbed.SetDescription", native_SetNewEmbedDescription},
    {"NewDiscordEmbed.AddField", native_NewEmbedAddField},
    {NULL, NULL}
};