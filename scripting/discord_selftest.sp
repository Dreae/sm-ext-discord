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

public void OnDiscordMessage(DiscordMessage msg) {
    char content[256];
    int authorId[2];
    char sAuthorId[24];

    msg.GetContent(content, sizeof(content));
    msg.AuthorId(authorId);
    UInt64ToString(authorId, sAuthorId);

    PrintToServer("Got message from %s (%d) (%d):  %s", sAuthorId, msg.IsSelf(), msg.IsBot(), content);
    PrintToServer("NumMentionedRoles: %d, NumMentionedUsers: %d, MentionsSelf: %d", msg.NumMentionedRoles(), msg.NumMentionedUsers(), msg.MentionsUser(g_iBotUserId));

    if (StrEqual("smtest", content)) {
        msg.ReplyToChannel("**test** *test* [test](https://google.com)");
    }

    if (StrEqual("embedtest", content)) {
        int channelId[2];
        msg.ChannelId(channelId);

        NewDiscordMessage newMsg = new NewDiscordMessage();
        NewDiscordEmbed embed = new NewDiscordEmbed();

        embed.SetTitle("**Test Embed**");
        embed.SetDescription("[Test description](https://google.com)");
        embed.AddField("Inline1", "Inline", true);
        embed.AddField("Inline2", "Inline", true);
        embed.AddField("Test", "[Testing](https://www.google.com)");


        newMsg.SetEmbed(embed);

        SendToChannel(channelId, newMsg);
    }
}