#include <sourcemod>
#include <discord.inc>

public Plugin myinfo = {
    name = "Discord Test",
    author = "Dreae <dreae@dreae.onl>",
    description = "Used for testing Discord",
    version = DISCORD_VERSION,
    url = "https://github.com/Dreae/sm-ext-discord"
}

int g_iBotUserId[2];

public void OnDiscordReady(DiscordReady ready) {
    ready.CurrentUserId(g_iBotUserId);
    char buffer[24];
    UInt64ToString(g_iBotUserId, buffer);

    PrintToServer("Bot user is %s", buffer);
}

public void OnDiscordMessage(DiscordUser author, DiscordMessage msg) {
    char content[256];
    char authorTag[128];

    msg.GetContent(content, sizeof(content));
    author.GetTag(authorTag, sizeof(authorTag));

    int channelId[2];
    int guildId[2];
    msg.ChannelId(channelId);
    msg.GuildId(guildId);
    char sChannelId[24];
    char sGuildId[24];
    UInt64ToString(channelId, sChannelId);
    UInt64ToString(guildId, sGuildId);

    PrintToServer("Got message from %s (%d) (%d):  %s", authorTag, msg.IsSelf(), msg.IsBot(), content);
    PrintToServer("NumMentionedRoles: %d, NumMentionedUsers: %d, MentionsSelf: %d", msg.NumMentionedRoles(), msg.NumMentionedUsers(), msg.MentionsUser(g_iBotUserId));
    PrintToServer("ChannelId: %s, GuildId: %s", sChannelId, sGuildId);

    int authorId[2];
    msg.AuthorId(authorId);
    Discord_FetchUser(authorId, On_GetUser, 1337);

    if (StrEqual("smtest", content)) {
        msg.ReplyToChannel("**test** *test* [test](https://google.com)");
    }

    if (StrEqual("embedtest", content)) {
        NewDiscordMessage newMsg = new NewDiscordMessage();
        NewDiscordEmbed embed = new NewDiscordEmbed();

        embed.SetTitle("**Test Embed**");
        embed.SetDescription("[Test description](https://google.com)");
        embed.AddField("Inline1", "Inline", true);
        embed.AddField("Inline2", "Inline", true);
        embed.AddField("Test", "[Testing](https://www.google.com)");


        newMsg.SetEmbed(embed);

        Discord_SendToChannel(channelId, newMsg);
    }
}

public void On_GetUser(DiscordUser user, any data) {
    char tag[128];
    user.GetTag(tag, sizeof(tag));
    PrintToServer("Got user info on %s: %d", tag, data);

    user.Close();
}