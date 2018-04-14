use std::ffi::CString;

use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use serenity::model::gateway::Ready;
use serenity::model::id::{ChannelId, UserId};
use ::model::{DiscordMessage, DiscordReady};
use ::client::ClientCallbacks;

pub struct Handler {
    plugin: usize
}

impl Handler {
    pub fn new(plugin: usize) -> Handler {
        Handler {
            plugin
        }
    }
}

impl EventHandler for Handler {
    fn message(&self, ctx: Context, msg: Message) {
        let data = ctx.data.lock();
        if let Some(ref callbacks) = data.get::<ClientCallbacks>() {
            if let Some(ref callback) = callbacks.msg_callback {
                let content = CString::new(msg.content).unwrap();
                let ChannelId(channel_id) = msg.channel_id;
                let UserId(author_id) = msg.author.id;
                let discord_message = DiscordMessage {
                    content: content.into_raw(),
                    channel_id,
                    author_id,
                    bot: if msg.author.bot { 1 } else { 0 }
                };

                if let Ok(callback) = callback.lock() {
                    ::glue::call_message_callback(*callback, self.plugin, &discord_message);
                }
            }
        }
    }

    fn ready(&self, ctx: Context, ready: Ready) {
        let data = ctx.data.lock();
        if let Some(ref callbacks) = data.get::<ClientCallbacks>() {
            if let Some(ref callback) = callbacks.ready_callback {
                let UserId(user_id) = ready.user.id;
                let discord_ready = DiscordReady {
                    user_id
                };

                if let Ok(callback) = callback.lock() {
                    ::glue::call_ready_callback(*callback, self.plugin, &discord_ready);
                }
            }
        }
    }
}