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
    public native void ReplyToChannel(const char[] content, any ...);

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

    /**
     * Retrieves the GuildId this message was received from
     * @note This returns 0 if the message was not sent to a
     *       guild channel
     *
     * @param id            The buffer to store the GuildId
     */
    public native void GuildId(int id[2]);

    /**
     * Determines if the supplied RoleId is mentioned in the
     * content of this message
     *
     * @param id            The RoleId to search for
     * @return              Was the role mentioned
     */
    public native bool MentionsRole(int id[2]);

    /**
     * Returns the number of roles mentioned in this message
     *
     * @return              The number of mentioned roles
     */
    public native int NumMentionedRoles();

    /**
     * Gets the RoleId for a mentioned role
     *
     * @param n             Which role to get the RoleId of
     * @param id            Buffer to store RoleId
     */
    public native void GetMentionedRole(int n, int id[2]);

    /**
     * Determines if the supplied UserId is mentioned in the
     * content of this message
     *
     * @param id            The UserId to search for
     * @return              Was the role mentioned
     */
    public native bool MentionsUser(int id[2]);

    /**
     * Returns the number of users mentioned in this message
     *
     * @return              The number of mentioned users
     */
    public native int NumMentionedUsers();

    /**
     * Gets the UserId for a mentioned user
     *
     * @param n             Which user to get the UserId of
     * @param id            Buffer to store UserId
     */
    public native void GetMentionedUser(int n, int id[2]);
}

methodmap DiscordReady < Handle {
    /**
     * Retrieves the current Discord UserID for this bot
     *
     * @param id            The buffer to store the UserID
     */
    public native void CurrentUserId(int id[2]);
}

methodmap DiscordUser < Handle {
    /**
     * Retrieves the name for this user
     *
     * @param buffer        Buffer to store the name
     * @param len           Maximum length of buffer
     */
    public native void GetName(char[] buffer, int len);

    /**
     * Retrieves the tag for this user
     *
     * @param buffer        Buffer to store the tag
     * @param len           Maximum length of buffer
     */
    public native void GetTag(char[] buffer, int len);

    /**
     * Checks if this user has a given role in a given guild
     *
     * @param guildId       The GuildId to use when searching for roles
     * @param roleId        The RoleId the user should have
     */
    public native bool HasRole(int guildId[2], int roleId[2]);
}

forward void OnDiscordMessage(DiscordUser author, DiscordMessage msg);
forward void OnDiscordReady(DiscordReady ready);

methodmap NewDiscordEmbed < Handle {
    /**
     * Creates a new embed object to represent the embed included
     * on a new Discord message
     *
     * @return the newly created embed
     */
    public native NewDiscordEmbed();

    /**
     * Sets the title for this embed object
     *
     * @param title         The title of this embed
     */
    public native void SetTitle(const char[] title, any ...);

    /**
     * Sets the description for this embed object
     *
     * @param description   The title of this embed
     */
    public native void SetDescription(const char[] description, any ...);

    /**
     * Adds a field to this embed object
     *
     * @param title         The title of the new field
     * @param value         The value for the new field
     * @param inline        Should the new field be displayed inline
     */
    public native void AddField(const char[] title, const char[] value, bool inline = false);

    /**
     * Sets the footer text for this embed object
     *
     * @param text          The text for the footer
     */
    public native void SetFooterText(const char[] text, any ...);

    /**
     * Sets the footer icon for this embed object
     *
     * @param icon          The icon url for the footer
     */
    public native void SetFooterIcon(const char[] icon);
}

methodmap NewDiscordMessage < Handle {
    /**
     *
     * Creates a new message for use with SendToChannel
     *
     * @return              The newly created message
     */
    public native NewDiscordMessage();

    /**
     * Sets the message content for this message
     *
     * @param content       The content to send to Discord
     */
    public native void SetContent(const char[] content, any ...);

    /**
     * Sets the embed to be included with this message
     * @note This frees the handle for embed, do not attempt
     *       to use the embed after calling this method
     *
     * @param embed         The embeds to include in this message
     */
    public native void SetEmbed(NewDiscordEmbed embed);
}

typeset FetchUserCallback {
    function void(DiscordUser user);
    function void(DiscordUser user, any data);
}

/**
 * Converts a UInt64 to a string for easier storage
 *
 * @param u64           The UInt64 to convert
 * @param buffer        The buffer to store the converted value
 */
native void UInt64ToString(int u64[2], char buffer[24]);

/**
 * Parses a UInt64 from a string
 *
 * @param buffer        The buffer to parse
 * @param u64           The buffer to store the parsed value
 */
native void StringToUInt64(const char[] buffer, int u64[2]);

/**
 * Sends a rich message to the specified ChannelId
 * @note This closes the handle for message, do not attempt
 *       to reuse the NewDiscordMessage after calling SendToChannel
 *
 * @param channel_id        The ChannelId to send this message to
 * @param message           The message to send to the channel
 */
native void Discord_SendToChannel(int channel_id[2], NewDiscordMessage message);

/**
 * Fetches information about a user by UserId
 *
 * @param userId            The UserId to lookup
 * @param callback          The function to call with the retrieved data
 */
native void Discord_FetchUser(int userId[2], FetchUserCallback callback, any data = 0);

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