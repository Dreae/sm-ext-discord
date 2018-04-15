#if defined _discord_included
  #endinput
#endif
#define _discord_included

#define DISCORD_VERSION "0.0.1"

methodmap DiscordMessage < Handle {
    /**
     * Gets the content of this message
     *
     * @param buffer        The buffer to store the messsage
     * @param buflen        The maximum size to store
     */
    public native void GetContent(char[] buffer, int buflen);

    /**
     * Sends a message to the same channel this message was
     * received from
     *
     * @param content       The content to send to the channel
     */
    public native void ReplyToChannel(const char[] content);

    /**
     * Indicates whether the message was sent by a bot
     *
     * @return              Was the message sent by a bot
     */
    public native bool IsBot();

    /**
     * Indicates whether this message was sent by this bot
     *
     * @return              Was the message sent by this bot
     */
    public native bool IsSelf();

    /**
     * Retrieves the UserId of the message author
     *
     * @param id            The buffer to store the UserId
     */
    public native void AuthorId(int id[2]);

    /**
     * Retrieves the ChannelId the message was received from
     *
     * @param id            The buffer to store the ChannelId
     */
    public native void ChannelId(int id[2]);
}

methodmap DiscordReady < Handle {
    /**
     * Retrieves the current Discord UserID for this bot
     *
     * @param id            The buffer to store the UserID
     */
    public native void CurrentUserId(int id[2]);
}

typedef DiscordMessageCallback = function void (DiscordMessage msg);
typedef DiscordReadyCallback = function void (DiscordReady ready);

methodmap DiscordClient < Handle {
    /**
     * Constructs a new DiscordClient
     *
     * @param token         The bot token to use to connect
     *                      to discord
     * @return              A Handle to the new DiscordClient
     */
    public native DiscordClient(const char[] token);

    /**
     * Sets the callback that will be called when this client
     * receives a message from Discord
     *
     * @param cb            The callback to be called
     */
    public native void SetMessageCallback(DiscordMessageCallback cb);

    /**
     * Sets the callback that will be called when this client
     * is connected to Discord and ready to start processing
     * events.
     *
     * @param cb            The callback to be called
     */
    public native void SetReadyCallback(DiscordReadyCallback cb);

    /**
     * Connect this client to Discord to actually start
     * processing messages
     */
    public native void Connect();
}

methodmap NewDiscordEmbed < Handle {
    public native NewDiscordEmbed();

    public native void SetTitle(const char[] title);

    public native void SetDescription(const char[] description);
}

methodmap NewDiscordMessage < Handle {
    public native NewDiscordMessage();

    public native void SetContent(const char[] content);

    public native void SetEmbed(NewDiscordEmbed embed);
}

/**
 * Converts a UInt64 to a string for easier storage
 *
 * @param u64           The UInt64 to convert
 * @param buffer        The buffer to store the converted value
 */
native void UInt64ToString(int u64[2], char buffer[20]);

/**
 * Parses a UInt64 from a string
 *
 * @param buffer        The buffer to parse
 * @param u64           The buffer to store the parsed value
 */
native void StringToUint64(const char[] buffer, int u64[2]);

native void SendToChannel(int channel_id[2], NewDiscordMessage message);

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