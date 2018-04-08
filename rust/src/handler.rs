use std::ffi::CString;
use std::os::raw::c_void;
use std::sync::Mutex;
use std::ptr::Unique;

use serenity::prelude::{EventHandler, Context};
use serenity::model::channel::Message;
use ::model::DiscordMessage;

pub struct Handler {
    msg_callback: Option<Mutex<Unique<c_void>>>
}

impl Handler {
    pub fn new() -> Handler {
        Handler {
            msg_callback: None
        }
    }

    pub fn set_callback(&mut self, callback: *mut c_void) {
        self.msg_callback = Some(Mutex::new(Unique::new(callback).unwrap()));
    }
}

impl EventHandler for Handler {
    fn message(&self, _: Context, msg: Message) {
        if let Some(ref callback) = self.msg_callback {
            let discord_message = DiscordMessage {
                content: CString::new(msg.content.as_str()).unwrap().into_raw()
            };
            let msg_ptr = Box::into_raw(Box::new(discord_message));

            if let Ok(callback) = callback.lock() {
                unsafe {
                    ::glue::call_message_callback((*callback).as_ptr(), msg_ptr);
                }
            }

            unsafe {
                Box::from_raw(msg_ptr);
            }
        }
    }
}