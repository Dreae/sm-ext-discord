#![feature(ptr_internals)]
extern crate serenity;

use std::os::raw::{c_void, c_char};

use std::ffi::CStr;
use std::thread;

mod handler;
mod model;
mod glue;

use handler::Handler;

use serenity::prelude::*;
use serenity::model::id::ChannelId;

pub use model::free_discord_message;

#[no_mangle]
pub extern "C" fn create_handler(plugin: *const c_void) -> *mut c_void {
    Box::into_raw(Box::new(Handler::new(plugin as usize))) as *mut c_void
}

#[no_mangle]
pub extern "C" fn handler_set_msg_callback(handler: *mut c_void, callback: *mut c_void) {
    unsafe {
        let handler = &mut *(handler as *mut Handler);
        handler.set_callback(callback);
    }
}

#[no_mangle]
pub extern "C" fn connect_handler(handler: *mut c_void, token: *const c_char) {
    unsafe {
        let c_str = CStr::from_ptr(token);
        match c_str.to_str() {
            Ok(token) => {
                let handler = *Box::from_raw(handler as *mut Handler);

                match Client::new(token, handler) {
                    Ok(mut client) => {
                        thread::spawn(move || {
                            if let Err(err) = client.start() {
                                glue::log_error(&format!("Client error: {:?}", err));
                            }
                        });
                    },
                    Err(err) => {
                        glue::log_error(&format!("Connection error: {:?}", err));
                    }
                }
            },
            _ => {
                glue::log_error("Invalid bot token provided");
            }
        }
    }
}

#[no_mangle]
pub extern "C" fn say_to_channel(channel_id: u64, msg: *const c_char) {
    unsafe {
        let c_str = CStr::from_ptr(msg);
        if let Ok(msg) = c_str.to_str() {
            let channel_id = ChannelId(channel_id);
            if let Err(err) = channel_id.say(msg) {
                glue::log_error(&format!("There was an error sending message: {:?}", err));
            }
        }
    }
}