use std::ffi::CString;

use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use serenity::model::gateway::Ready;
use serenity::model::id::{ChannelId, RoleId, UserId};
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
                let own = if msg.is_own() { 1 } else { 0 };
                let content = CString::new(msg.content).unwrap();
                let ChannelId(channel_id) = msg.channel_id;
                let UserId(author_id) = msg.author.id;

                let mentioned_roles: Vec<u64> = msg.mention_roles.into_iter().map(|role_id| {
                    let RoleId(role) = role_id;
                    role
                }).collect();
                let mentioned_users: Vec<u64> = msg.mentions.into_iter().map(|user| {
                    let UserId(user_id) = user.id;
                    user_id
                }).collect();

                let discord_message = DiscordMessage {
                    content: content.as_ptr(),
                    channel_id,
                    author_id,
                    bot: if msg.author.bot { 1 } else { 0 },
                    own,
                    mentioned_roles: mentioned_roles.as_ptr(),
                    num_mentioned_roles: mentioned_roles.len() as u32,
                    mentioned_users: mentioned_users.as_ptr(),
                    num_mentioned_users: mentioned_users.len() as u32
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