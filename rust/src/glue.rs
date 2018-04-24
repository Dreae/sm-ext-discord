use serenity::model::gateway::Ready;
use serenity::model::channel::Message;
use std::ffi::CString;
use serenity::model::user::User;
use std::os::raw::c_void;

type DiscordMessage = c_void;
type DiscordReady = c_void;
type DiscordUser = c_void;

pub fn call_message_callback(author: Box<User>, msg: Box<Message>) {
    unsafe {
        c::call_message_callback(Box::into_raw(author) as *const DiscordUser, Box::into_raw(msg) as *const DiscordMessage);
    }
}

pub fn call_ready_callback(ready: Box<Ready>) {
    unsafe {
        c::call_ready_callback(Box::into_raw(ready) as *const DiscordReady);
    }
}

pub fn call_user_callback(user: Box<User>, callback: usize, plugin: usize, data: i32) {
    unsafe {
        c::call_user_callback(Box::into_raw(user) as *mut DiscordUser, callback as *const c_void, plugin as *const c_void, data);
    }
}

pub fn log_error(msg: &str) {
    let msg = CString::new(msg).unwrap();
    unsafe {
        c::log_error(msg.as_ptr());
    }
}

pub fn log_message(msg: &str) {
    let msg = CString::new(msg).unwrap();
    unsafe {
        c::log_message(msg.as_ptr());
    }
}

mod c {
    use std::os::raw::{c_void, c_char};
    use super::{DiscordMessage, DiscordReady, DiscordUser};

    extern "C" {
        pub fn call_message_callback(user: *const DiscordUser, msg: *const DiscordMessage);
        pub fn call_ready_callback(ready: *const DiscordReady);
        pub fn call_user_callback(user: *const DiscordUser, callback: *const c_void, plugin: *const c_void, data: i32);
        pub fn log_error(msg: *const c_char);
        pub fn log_message(msg: *const c_char);
    }
}
