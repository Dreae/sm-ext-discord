extern crate serenity;
extern crate typemap;

use std::os::raw::{c_void, c_char};

use std::ffi::CStr;

mod handler;
mod model;
mod glue;
mod client;

use handler::Handler;
use client::DiscordClient;

use serenity::model::id::ChannelId;

pub use model::free_discord_message;

#[no_mangle]
pub extern "C" fn create_client(plugin: *const c_void, token: *const c_char) -> *mut c_void {
    let handler = Handler::new(plugin as usize);
    let c_str = unsafe { CStr::from_ptr(token).to_owned() };
     match c_str.to_str() {
        Ok(token) => {
            if let Some(client) = DiscordClient::new(token, handler) {
                Box::into_raw(Box::new(client)) as *mut c_void
            } else {
                std::ptr::null_mut()
            }
        },
        _ => {
            glue::log_error("Invalid discord token provided");
            std::ptr::null_mut()
        }
     }
}

#[no_mangle]
pub extern "C" fn client_set_msg_callback(client: *mut c_void, callback: *mut c_void) {
    unsafe {
        let client = &mut *(client as *mut DiscordClient);
        client.set_message_callback(callback as usize);
    }
}

#[no_mangle]
pub extern "C" fn connect_client(client: *mut c_void) {
    unsafe {
        let client = &mut *(client as *mut DiscordClient);
        client.start();
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