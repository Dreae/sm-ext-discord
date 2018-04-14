#include <sourcemod>
#include <discord.inc>

public Plugin myinfo = {
    name = "Discord Test",
    author = "Dreae <dreae@dreae.onl>",
    description = "Used for testing Discord",
    version = DISCORD_VERSION,
    url = "https://github.com/Dreae/sm-ext-discord"
}

ConVar g_hCvarBotToken;
char g_sDiscordToken[256];
bool g_bConnected = false;
int g_iBotUserId[2];

public void OnPluginStart() {
    g_hCvarBotToken = CreateConVar("discord_bot_token", "", "Discord bot token", FCVAR_PROTECTED);
}

public void OnConfigsExecuted() {
    g_hCvarBotToken.GetString(g_sDiscordToken, sizeof(g_sDiscordToken));
    PrintToServer("Got bot token %s", g_sDiscordToken);
    if (strlen(g_sDiscordToken) != 0 && !g_bConnected) {
        DiscordClient client = new DiscordClient(g_sDiscordToken);
        client.SetMessageCallback(On_DiscordMessage);
        client.SetReadyCallback(On_DiscordReady);
        client.Connect();
        g_bConnected = true;
    }
}

public void On_DiscordReady(DiscordReady ready) {
    ready.CurrentUserId(g_iBotUserId);

    PrintToServer("Bot user is %08x%08x", g_iBotUserId[1], g_iBotUserId[0]);
}

public void On_DiscordMessage(DiscordMessage msg) {
    char content[256];
    int authorId[2];

    msg.GetContent(content, sizeof(content));
    msg.AuthorId(authorId);

    PrintToServer("Got message from %08x%08x:  %s", authorId[1], authorId[0], content);
}