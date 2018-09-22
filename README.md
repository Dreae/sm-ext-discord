## SM Discord
`sm-ext-discord` provides an easy way to create SourceMod plugins that function as bots in Discord, providing both the ability to send *and receive* messages from Discord users. The extension sets up a websocket connection to Discord to act as a bot, and exposes Discord actions and events through a SourcePawn API.

### Installation
 1. Download a pre-built extension binary (see below).
 2. Extract the downloaded extension to your SourceMod folder.
 3. Set up a Discord bot through Discord's website and copy the bot token.
 4. [Add the bot to your server](https://discordapp.com/developers/docs/topics/oauth2#bots)
 5. Set the `discord_bot_token` console variable to the token copied in step 2.
 6. Restart the server or manually load the extension.

This exension does nothing on its own, it only provides methods for writing plugins that have Discord functionality. For examples of plugins check the scripting folder.

### Pre-Build Binaries
| Game       | Windows                                                                                | Linux                                                                                          |
|------------|----------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------|
| TF2        | [AppVeyor](https://ci.appveyor.com/project/Dreae/sm-ext-discord-h6m1p/build/artifacts) | [GitLab](https://gitlab.com/Dreae/sm-ext-discord/-/jobs/artifacts/master/download/?job=bundle) |
| CS:GO      | [AppVeyor](https://ci.appveyor.com/project/Dreae/sm-ext-discord/build/artifacts)       | [GitLab](https://gitlab.com/Dreae/sm-ext-discord/-/jobs/artifacts/master/download/?job=bundle) |
| CS:S       | [AppVeyor](https://ci.appveyor.com/project/Dreae/sm-ext-discord-cw90i/build/artifacts) | [GitLab](https://gitlab.com/Dreae/sm-ext-discord/-/jobs/artifacts/master/download/?job=bundle) |
| Insurgency | [AppVeyor](https://ci.appveyor.com/project/Dreae/sm-ext-discord-tn4it/build/artifacts) | [GitLab](https://gitlab.com/Dreae/sm-ext-discord/-/jobs/artifacts/master/download/?job=bundle) |