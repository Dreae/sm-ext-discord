use ::model::{DiscordMessage, DiscordReady};
use std::ffi::CString;
use serenity::model::user::User;
use std::os::raw::c_void;

pub fn call_message_callback(msg: &DiscordMessage) {
    unsafe {
        c::call_message_callback(msg as *const DiscordMessage);
    }
}

pub fn call_ready_callback(ready: &DiscordReady) {
    unsafe {
        c::call_ready_callback(ready as *const DiscordReady);
    }
}

pub fn call_user_callback(user: *mut User, callback: usize, plugin: usize, data: i32) {
    unsafe {
        c::call_user_callback(user as *mut c_void, callback as *const c_void, plugin as *const c_void, data);
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
    use ::model::{DiscordMessage, DiscordReady};

    extern "C" {
        pub fn call_message_callback(msg: *const DiscordMessage);
        pub fn call_ready_callback(ready: *const DiscordReady);
        pub fn call_user_callback(user: *mut c_void, callback: *const c_void, plugin: *const c_void, data: i32);
        pub fn log_error(msg: *const c_char);
        pub fn log_message(msg: *const c_char);
    }
}
