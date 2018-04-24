use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use serenity::model::gateway::Ready;

pub struct Handler;

impl Handler {
    pub fn new() -> Handler {
        Handler
    }
}

impl EventHandler for Handler {
    fn message(&self, _ctx: Context, msg: Message) {
        ::glue::call_message_callback(Box::new(msg.author.clone()), Box::new(msg));
    }

    fn ready(&self, _ctx: Context, ready: Ready) {
        ::glue::call_ready_callback(Box::new(ready));
    }
}