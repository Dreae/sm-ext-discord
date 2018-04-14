#include <sourcemod>
#include "include/discord.inc"

public Plugin myinfo = {
    name = "Discord Test",
    author = "Dreae <dreae@dreae.onl>",
    description = "Used for testing Discord",
    version = DISCORD_VERSION,
    url = "https://github.com/Dreae/sm-ext-discord"
}

ConVar g_hCvarBotToken;
char g_sDiscordToken[128];

public void OnPluginStart() {
    g_hCvarBotToken = CreateConVar("discord_bot_token", "", "Discord bot token", FCVAR_PROTECTED);
}

public OnConfigsExecuted() {
    g_hCvarBotToken.GetString(g_sDiscordToken, sizeof(g_sDiscordToken));
    if (strlen(g_sDiscordToken) != 0) {
        DiscordClient client = new DiscordClient();
        client.SetMessageCallback(On_DiscordMessage);
        client.Connect(g_sDiscordToken);
    }
}

public void On_DiscordMessage(DiscordMessage msg) {
    char content[256];

    msg.GetContent(content, sizeof(content));
    PrintToServer("Got message %s", content);
}