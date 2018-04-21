use ::model::{DiscordMessage, DiscordReady};
use std::ffi::CString;

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
    use std::os::raw::c_char;
    use ::model::{DiscordMessage, DiscordReady};

    extern "C" {
        pub fn call_message_callback(msg: *const DiscordMessage);
        pub fn call_ready_callback(ready: *const DiscordReady);
        pub fn log_error(msg: *const c_char);
        pub fn log_message(msg: *const c_char);
    }
}
