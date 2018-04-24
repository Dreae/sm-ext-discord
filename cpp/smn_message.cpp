#include "Extension.hpp"
#include "handletypes.hpp"
#include "include/rust.h"


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

        g_MessageType = handlesys->CreateType("DiscordMessage", this, 0, &tacc, &hacc, myself->GetIdentity(), NULL);
        sharesys->AddNatives(myself, discord_message_natives);
    }

    void OnExtUnload() {
        handlesys->RemoveType(g_MessageType, myself->GetIdentity());
    }

    void OnHandleDestroy(HandleType_t type, void *object) {
        free_discord_message(object);
    }

    // Not even going to try to estimate the size of the underlying Rust object
    bool GetHandleApproxSize(HandleType_t type, void* object, unsigned int *pSize) {
        return false;
    }
};

DiscordMessageNatives messageNatives;

static cell_t native_GetMessageContent(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);

    get_message_content(msg, reinterpret_cast<char *>(addr), params[3]);

    return 1;
}

static cell_t native_ReplyToChannel(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    char msg_content[2000];
    {
        DetectExceptions eh(pContext);
        size_t len = smutils->FormatString(msg_content, sizeof(msg_content) - 1, pContext, params, 2);
        if (eh.HasException()) {
            return 1;
        }
        msg_content[len] = '\0';
    }

    say_to_channel(get_message_channel_id(msg), msg_content);

    return 1;
}

static cell_t native_IsBot(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    return get_message_is_bot(msg);
}

static cell_t native_IsSelf(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    return get_message_is_self(msg);
}

static cell_t native_AuthorId(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    *reinterpret_cast<u64_t *>(addr) = get_message_author_id(msg);

    return 1;
}

static cell_t native_ChannelId(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    *reinterpret_cast<u64_t *>(addr) = get_message_channel_id(msg);

    return 1;
}

static cell_t native_GuildId(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    *reinterpret_cast<u64_t *>(addr) = get_message_guild_id(msg);

    return 1;
}

static cell_t native_MentionsRole(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    u64_t id = *reinterpret_cast<u64_t *>(addr);

    return get_message_mentions_role(msg, id);
}

static cell_t native_NumMentionedRoles(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    return get_message_num_mentioned_roles(msg);
}

static cell_t native_GetMentionedRole(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t which = params[2];
    if (which >= get_message_num_mentioned_roles(msg)) {
        pContext->ReportError("Index out of bounds: %d", which);
        return 0;
    }

    cell_t *addr;
    pContext->LocalToPhysAddr(params[3], &addr);
    *reinterpret_cast<u64_t *>(addr) = get_message_mentioned_role(msg, which);

    return 1;
}

static cell_t native_MentionsUser(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    u64_t id = *reinterpret_cast<u64_t *>(addr);

    return get_message_mentions_user(msg, id);
}

static cell_t native_NumMentionedUsers(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    return get_message_num_mentioned_users(msg);
}

static cell_t native_GetMentionedUser(IPluginContext *pContext, const cell_t *params) {
    DiscordMessage *msg = ReadHandle<DiscordMessage>(pContext, params[1], g_MessageType);

    cell_t which = params[2];
    if (which >= get_message_num_mentioned_users(msg)) {
        pContext->ReportError("Index out of bounds: %d", which);
        return 0;
    }

    cell_t *addr;
    pContext->LocalToPhysAddr(params[3], &addr);
    *reinterpret_cast<u64_t *>(addr) = get_message_mentioned_user(msg, which);

    return 1;
}

const sp_nativeinfo_t discord_message_natives[] = {
    {"DiscordMessage.GetContent", native_GetMessageContent},
    {"DiscordMessage.ReplyToChannel", native_ReplyToChannel},
    {"DiscordMessage.IsBot", native_IsBot},
    {"DiscordMessage.IsSelf", native_IsSelf},
    {"DiscordMessage.AuthorId", native_AuthorId},
    {"DiscordMessage.ChannelId", native_ChannelId},
    {"DiscordMessage.GuildId", native_GuildId},
    {"DiscordMessage.MentionsRole", native_MentionsRole},
    {"DiscordMessage.NumMentionedRoles", native_NumMentionedRoles},
    {"DiscordMessage.GetMentionedRole", native_GetMentionedRole},
    {"DiscordMessage.MentionsUser", native_MentionsUser},
    {"DiscordMessage.NumMentionedUsers", native_NumMentionedUsers},
    {"DiscordMessage.GetMentionedUser", native_GetMentionedUser},
    {NULL, NULL}
};