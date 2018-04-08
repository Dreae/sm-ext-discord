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

pub use model::free_discord_message;

#[no_mangle]
pub extern "C" fn create_handler() -> *mut c_void {
    Box::into_raw(Box::new(Handler::new())) as *mut c_void
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
        let token_str = c_str.to_owned();
        let token = token_str.to_str().unwrap();

        let handler = *Box::from_raw(handler as *mut Handler);

        let mut client = Client::new(token, handler).unwrap();

        thread::spawn(move || {
            client.start().unwrap();
        });
    }
}
