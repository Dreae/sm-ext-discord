#if defined _discord_included
  #endinput
#endif
#define _discord_included

#define DISCORD_VERSION "0.0.1"

methodmap DiscordMessage < Handle {
    public native void GetContent(char[] buffer, int buflen);

    public native void ReplyToChannel(char[] content);
}

typedef DiscordMessageCallback = function void (DiscordMessage msg);

methodmap DiscordClient < Handle {
    public native DiscordClient(const char[] token);

    public native void SetMessageCallback(DiscordMessageCallback cb);

    public native void Connect();
}

public Extension:__ext_smdiscord = {
    name = "Discord",
    file = "discord.ext",
#if defined AUTOLOAD_EXTENSIONS
    autoload = 1,
#else
    autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};