#pragma once

#include "include/smsdk_ext.h"
#include "include/rust.h"
#include <convar.h>
#include "work_queue.hpp"

#define LOG_MESSAGE(format, ...) \
  smutils->LogMessage(myself, format, ##__VA_ARGS__);

template <typename T>
inline T *ReadHandle(IPluginContext *pContext, const cell_t handle, HandleType_t handleType) {
    Handle_t hndl = static_cast<Handle_t>(handle);

    HandleSecurity sec;
    T *obj;

    sec.pOwner = pContext->GetIdentity();
    sec.pIdentity = myself->GetIdentity();
    auto herr = handlesys->ReadHandle(hndl, handleType, &sec, reinterpret_cast<void **>(&obj));

    if (herr != HandleError_None) {
        pContext->ReportError("Invalid Client handle %x (error %d)", hndl, herr);
        return nullptr;
    }
    return obj;
}

class Extension : public SDKExtension, public IConCommandBaseAccessor {
public:
  virtual bool SDK_OnLoad(char *error, size_t err_max, bool late);
  virtual void SDK_OnUnload();
  virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late);
  virtual void OnCoreMapStart(edict_t *pEdictList, int edictCount, int clientMax);
public: // IConCommandBaseAccessor
  bool RegisterConCommandBase(ConCommandBase *pVar);
};

class BaseClass {
  friend class Extension;
public:
  BaseClass() {
    next = BaseClass::head;
    BaseClass::head = this;
  }
  virtual void OnExtLoad() { };
  virtual void OnExtUnload() { };
private:
  BaseClass *next;
  static BaseClass *head;
};

extern Extension extension;
extern IForward *g_MessageForward;
extern IForward *g_ReadyForward;
extern ConVar discord_bot_token;
