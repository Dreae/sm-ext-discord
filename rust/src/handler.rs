use std::ffi::CString;
use std::mem;

use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use serenity::model::gateway::Ready;
use serenity::model::id::{ChannelId, RoleId, UserId};
use ::model::{DiscordMessage, DiscordReady};

pub struct Handler;

impl Handler {
    pub fn new() -> Handler {
        Handler
    }
}

impl EventHandler for Handler {
    fn message(&self, _ctx: Context, msg: Message) {
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

        let author = Box::new(msg.author);

        let discord_message = DiscordMessage {
            content: content.as_ptr(),
            channel_id,
            author_id,
            bot: if author.bot { 1 } else { 0 },
            own,
            mentioned_roles: mentioned_roles.as_ptr(),
            num_mentioned_roles: mentioned_roles.len() as u32,
            mentioned_users: mentioned_users.as_ptr(),
            num_mentioned_users: mentioned_users.len() as u32
        };

        ::glue::call_message_callback(Box::into_raw(author), &discord_message);
    }

    fn ready(&self, _ctx: Context, ready: Ready) {
        let UserId(user_id) = ready.user.id;
        let discord_ready = DiscordReady {
            user_id
        };

        ::glue::call_ready_callback(&discord_ready);
    }
}