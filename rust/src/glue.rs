use std::os::raw::c_void;
use ::model::DiscordMessage;

extern "C" {
    pub fn call_message_callback(callback: *const c_void, msg: *const DiscordMessage);
}