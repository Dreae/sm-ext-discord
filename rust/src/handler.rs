use std::ffi::CString;

use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use serenity::model::id::ChannelId;
use ::model::DiscordMessage;
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
                let discord_message = DiscordMessage {
                    content: content.into_raw(),
                    channel_id: channel_id
                };

                if let Ok(callback) = callback.lock() {
                    ::glue::call_message_callback(*callback, self.plugin, &discord_message);
                }
            }
        }
    }
}