#![feature(conservative_impl_trait)]

#[macro_use]
extern crate lazy_static;

extern crate serenity;
extern crate typemap;
extern crate rayon;

use std::os::raw::c_char;
use std::thread;
use std::ffi::CStr;

mod handler;
mod model;
mod glue;
mod client;

use serenity::model::id::ChannelId;
use rayon::{ThreadPool, ThreadPoolBuilder};

pub use model::c::*;
pub use client::c::*;

lazy_static! {
    pub static ref MSG_POOL: ThreadPool = ThreadPoolBuilder::new().num_threads(2).build().unwrap();
}

#[no_mangle]
pub extern "C" fn say_to_channel(channel_id: u64, msg: *const c_char) {
    unsafe {
        let c_str = CStr::from_ptr(msg);
        if let Ok(msg) = c_str.to_str() {
            let channel_id = ChannelId(channel_id);
            // TODO: Thread pooling?
            // TODO: Move this to a common module
            MSG_POOL.spawn(move || {
                if let Err(err) = channel_id.say(msg) {
                    glue::log_error(&format!("There was an error sending message: {:?}", err));
                }
            });
        }
    }
}