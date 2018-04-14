use std::ffi::CString;
use std::os::raw::c_void;
use std::sync::Mutex;

use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use ::model::DiscordMessage;

pub struct Handler {
    plugin: usize,
    msg_callback: Option<Mutex<usize>>
}

impl Handler {
    pub fn new(plugin: usize) -> Handler {
        Handler {
            plugin,
            msg_callback: None
        }
    }

    pub fn set_callback(&mut self, callback: *mut c_void) {
        self.msg_callback = Some(Mutex::new(callback as usize));
    }
}

impl EventHandler for Handler {
    fn message(&self, _: Context, msg: Message) {
        if let Some(ref callback) = self.msg_callback {
            let discord_message = DiscordMessage {
                content: CString::new(msg.content.as_str()).unwrap().into_raw()
            };

            if let Ok(callback) = callback.lock() {
                ::glue::call_message_callback(*callback, self.plugin, &discord_message);
            }
        }
    }
}