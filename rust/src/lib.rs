#![feature(conservative_impl_trait)]

#[macro_use]
extern crate lazy_static;

extern crate serenity;
extern crate typemap;
extern crate rayon;

use std::os::raw::c_char;
use std::ffi::CStr;
use std::sync::atomic::{AtomicBool, Ordering};

mod handler;
mod model;
mod glue;
mod client;

use serenity::model::id::ChannelId;
use rayon::{ThreadPool, ThreadPoolBuilder};
use client::DiscordClient;
use handler::Handler;

pub use model::c::*;

lazy_static! {
    pub static ref MSG_POOL: ThreadPool = ThreadPoolBuilder::new().num_threads(2).build().unwrap();
    pub static ref CONNECTED: AtomicBool = AtomicBool::new(false);
}

#[no_mangle]
pub extern "C" fn start_discord_client(token: *const c_char) {
    if !CONNECTED.load(Ordering::Acquire) {
        let handler = Handler::new();
        let c_str = unsafe { CStr::from_ptr(token).to_owned() };
         match c_str.to_str() {
            Ok(token) => {
                if let Some(client) = DiscordClient::new(token, handler) {
                    client.start();
                    glue::log_message("Discord client started");
                    CONNECTED.store(true, Ordering::Release);
                } else {
                    glue::log_error("Error connecting to discord");
                }
            },
            _ => {
                glue::log_error("Invalid discord token provided");
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
            MSG_POOL.spawn(move || {
                if let Err(err) = channel_id.say(msg) {
                    glue::log_error(&format!("There was an error sending message: {:?}", err));
                }
            });
        }
    }
}