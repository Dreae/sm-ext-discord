#![feature(libc)]

#[macro_use]
extern crate lazy_static;
#[macro_use]
extern crate serde_derive;

extern crate serenity;
extern crate typemap;
extern crate rayon;
extern crate libc;
extern crate serde;
extern crate serde_json;

#[cfg(not(windows))]
extern crate openssl_probe;

use std::os::raw::c_char;
use std::ffi::CStr;
use std::sync::atomic::{AtomicBool, Ordering};

mod handler;
mod model;
mod glue;
mod client;

use serenity::model::id::ChannelId;
use serenity::http;

use serde_json::Value;

use rayon::{ThreadPool, ThreadPoolBuilder};
use client::DiscordClient;
use handler::Handler;
use model::SendableDiscordMessage;

pub use model::c::*;

pub mod utils;

lazy_static! {
    pub static ref THREADPOOL: ThreadPool = ThreadPoolBuilder::new().num_threads(2).build().unwrap();
    pub static ref CONNECTED: AtomicBool = AtomicBool::new(false);
}

#[no_mangle]
pub extern "C" fn start_discord_client(token: *const c_char) {
    if !CONNECTED.load(Ordering::Acquire) {
        #[cfg(not(windows))]
        openssl_probe::init_ssl_cert_env_vars();

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
            THREADPOOL.spawn(move || {
                if let Err(err) = channel_id.say(msg) {
                    glue::log_error(&format!("There was an error sending message: {:?}", err));
                }
            });
        }
    }
}

#[no_mangle]
pub extern "C" fn execute_webhook(webhook_id: u64, token: *const c_char, message: *mut SendableDiscordMessage) {
    unsafe {
        let msg = Box::from_raw(message);
        let c_str = CStr::from_ptr(token);
        if let Ok(token) = c_str.to_str() {
            if let Ok(Value::Object(map)) = serde_json::to_value(msg) {
                THREADPOOL.spawn(move || {
                    if let Err(err) = http::execute_webhook(webhook_id, token, false, &map) {
                        glue::log_error(&format!("There was an error executing a webhook: {:?}", err));
                    }
                });
            }
        }
    }
}