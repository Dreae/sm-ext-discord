#include "Extension.hpp"
#include "handletypes.hpp"
#include "include/rust.h"

#define READ_HANDLE(pContext, params) \
  Handle_t hndl = static_cast<Handle_t>(params[1]); \
  HandleSecurity sec; \
  DiscordMessage *obj; \
  sec.pOwner = pContext->GetIdentity(); \
  sec.pIdentity = myself->GetIdentity(); \
  auto herr = handlesys->ReadHandle(hndl, g_MessageType, &sec, reinterpret_cast<void **>(&obj)); \
  if (herr != HandleError_None) { \
    pContext->ReportError("Invalid Client handle %x (error %d)", hndl, herr); \
    return 0; \
  }


HandleType_t g_MessageType;
extern const sp_nativeinfo_t discord_message_natives[];

class DiscordMessageNatives : public BaseClass, public IHandleTypeDispatch {
public:
    void OnExtLoad() {
        HandleAccess hacc;
        TypeAccess tacc;

        handlesys->InitAccessDefaults(&tacc, &hacc);
        tacc.ident = myself->GetIdentity();
        hacc.access[HandleAccess_Read] = HANDLE_RESTRICT_OWNER;
        tacc.access[HTypeAccess_Create] = false;
        tacc.access[HTypeAccess_Inherit] = false;

        g_ClientType = handlesys->CreateType("DiscordMessage", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, discord_message_natives);
    }

    void OnExtUnload() {
        handlesys->RemoveType(g_ClientType, myself->GetIdentity());
    }

    void OnHandleDestory(HandleType_t type, void *object) {
        free_discord_message(reinterpret_cast<DiscordMessage *>(object));
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t, void* object, unsigned int *pSize) {
        return false;
    }
};

static cell_t native_GetMessageContent(IPluginContext *pContext, const cell_t *params) {
    READ_HANDLE(pContext, params);

    pContext->StringToLocal(params[2], params[3], obj->content);

    return 1;
}

const sp_nativeinfo_t discord_message_natives[] = {
    {"DiscordMessage.GetContent", native_GetMessageContent},
    {NULL, NULL}
};