#![feature(conservative_impl_trait)]

extern crate serenity;
extern crate typemap;

use std::os::raw::c_char;

use std::ffi::CStr;

mod handler;
mod model;
mod glue;
mod client;


use serenity::model::id::ChannelId;

pub use model::c::*;
pub use client::c::*;

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