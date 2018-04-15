#include "Extension.hpp"
#include "include/rust.h"
#include <cstdlib>

extern const sp_nativeinfo_t smdiscord_natives[];

class DiscordNatives : public BaseClass {
public:
    void OnExtLoad() {
        sharesys->AddNatives(myself, smdiscord_natives);
    }
};

DiscordNatives natives;

static cell_t native_UInt64ToString(IPluginContext *pContext, const cell_t *params) {
    cell_t *value;
    pContext->LocalToPhysAddr(params[1], &value);
    u64_t val = *reinterpret_cast<u64_t *>(value);

    char *buffer;
    pContext->LocalToString(params[2], &buffer);

    snprintf(buffer, 20, "%llu", val);

    return 1;
}

static cell_t native_StringToUInt64(IPluginContext *pContext, const cell_t *params) {
    char *buffer;
    pContext->LocalToString(params[1], &buffer);

    u64_t value = strtoull(buffer, nullptr, 10);

    cell_t *addr;
    pContext->LocalToPhysAddr(params[2], &addr);
    *reinterpret_cast<u64_t *>(addr) = value;

    return 1;
}

const sp_nativeinfo_t smdiscord_natives[] = {
    {"UInt64ToString", native_UInt64ToString},
    {"StringToUInt64", native_StringToUInt64},
    {NULL, NULL}
};
