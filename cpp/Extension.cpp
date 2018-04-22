#include "Extension.hpp"

Extension extension;
SMEXT_LINK(&extension);

BaseClass *BaseClass::head = nullptr;
IForward *g_MessageForward = nullptr;
IForward *g_ReadyForward = nullptr;

ConVar discord_bot_token("discord_bot_token", "", FCVAR_PROTECTED, "Token to use to connect to Discord");

ICvar *icvar = NULL;

bool Extension::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late) {
    GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
#if SOURCE_ENGINE >= SE_ORANGEBOX
	g_pCVar = icvar;
	ConVar_Register(0, this);
#else
	ConCommandBaseMgr::OneTimeInit(this);
#endif

    return true;
}

bool Extension::SDK_OnLoad(char *error, size_t err_max, bool late) {
    ParamType types[] = {Param_Cell, Param_Cell};
    g_MessageForward = forwards->CreateForward("OnDiscordMessage", ET_Ignore, 2, types);
    g_ReadyForward = forwards->CreateForward("OnDiscordReady", ET_Ignore, 1, types);

    auto head = BaseClass::head;
    while (head) {
        head->OnExtLoad();
        head = head->next;
    }

    return true;
}

void Extension::SDK_OnUnload() {

}

void Extension::OnCoreMapStart(edict_t *pEdictList, int edictCount, int clientMax) {
    const char *token = discord_bot_token.GetString();
    if (strlen(token) != 0) {
        start_discord_client(discord_bot_token.GetString());
    }
}

bool Extension::RegisterConCommandBase(ConCommandBase *pVar) {
    return META_REGCVAR(pVar);
}
