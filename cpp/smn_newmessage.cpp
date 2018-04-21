#include "Extension.hpp"
#include "handletypes.hpp"
#include "include/rust.h"

HandleType_t g_NewMessageType;
extern const sp_nativeinfo_t newmessage_natives[];

class NewDiscordMessageNatives : public BaseClass, public IHandleTypeDispatch {
public:
    void OnExtLoad() {
        HandleAccess hacc;
        TypeAccess tacc;

        handlesys->InitAccessDefaults(&tacc, &hacc);
        tacc.ident = myself->GetIdentity();
        hacc.access[HandleAccess_Read] = HANDLE_RESTRICT_OWNER;

        g_NewMessageType = handlesys->CreateType("NewDiscordMessage", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, newmessage_natives);
    }
    void OnExtUnload() {
        handlesys->RemoveType(g_NewMessageType, myself->GetIdentity());
    }

    void OnHandleDestroy(HandleType_t type, void *object) {
        return;
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t type, void* object, unsigned int *pSize) {
        return false;
    }
};

NewDiscordMessageNatives new_message_natives;

static cell_t native_CreateNewDiscordMessage(IPluginContext *pContext, const cell_t *params) {
    auto new_message = create_new_discord_message();
    auto hndl = handlesys->CreateHandle(g_NewMessageType, new_message, pContext->GetIdentity(), myself->GetIdentity(), NULL);

    return hndl;
}

static cell_t native_SetNewMessageContent(IPluginContext *pContext, const cell_t *params) {
    auto new_message = ReadHandle<NewDiscordMessage>(pContext, params[1], g_NewMessageType);

    char content[2000];
    DetectExceptions eh(pContext);
    {
        size_t len = smutils->FormatString(content, sizeof(content) - 1, pContext, params, 2);
        if (eh.HasException()) {
            return 0;
        }
        content[len] = '\0';
    }

    set_new_message_content(new_message, content);

    return 1;
}

static cell_t native_SetNewMessageEmbed(IPluginContext *pContext, const cell_t *params) {
    auto new_message = ReadHandle<NewDiscordMessage>(pContext, params[1], g_NewMessageType);
    auto new_embed = ReadHandle<NewDiscordEmbed>(pContext, params[2], g_NewEmbedType);

    if (!new_embed) {
        pContext->ReportError("Invalid embed handle provided");

        return 0;
    }

    set_new_message_embed(new_message, new_embed);

    HandleSecurity sec;
    sec.pOwner = pContext->GetIdentity();
    sec.pIdentity = myself->GetIdentity();
    handlesys->FreeHandle(static_cast<Handle_t>(params[2]), &sec);

    return 1;
}

const sp_nativeinfo_t newmessage_natives[] = {
    {"NewDiscordMessage.NewDiscordMessage", native_CreateNewDiscordMessage},
    {"NewDiscordMessage.SetContent", native_SetNewMessageContent},
    {"NewDiscordMessage.SetEmbed", native_SetNewMessageEmbed},
    {NULL, NULL}
};