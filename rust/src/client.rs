use std::sync::Mutex;
use std::thread;

use typemap::Key;
use serenity::client::Client;
use ::handler::Handler;
use ::glue;

#[derive(Default)]
pub struct ClientCallbacks {
    pub msg_callback: Option<Mutex<usize>>,
    pub ready_callback: Option<Mutex<usize>>
}

impl Key for ClientCallbacks {
    type Value = ClientCallbacks;
}

pub struct DiscordClient {
    client: Client
}

impl DiscordClient {
    pub fn new(token: &str, handler: Handler) -> Option<DiscordClient> {
        match Client::new(token, handler) {
            Ok(client) => {
                {
                    let mut data = client.data.lock();
                    data.insert::<ClientCallbacks>(Default::default());
                }

                Some(DiscordClient {
                    client
                })
            },
            Err(err) => {
                glue::log_error(&format!("Connection error: {:?}",  err));
                None
            }
        }
    }

    pub fn set_message_callback(&mut self, callback: usize) {
        let mut data = self.client.data.lock();
        if let Some(callbacks) = data.get_mut::<ClientCallbacks>() {
            callbacks.msg_callback = Some(Mutex::new(callback));
        }
    }

    pub fn set_ready_callback(&mut self, callback: usize) {
        let mut data = self.client.data.lock();
        if let Some(callbacks) = data.get_mut::<ClientCallbacks>() {
            callbacks.ready_callback = Some(Mutex::new(callback));
        }
    }

    pub fn start(&'static mut self) {
        thread::spawn(move || {
            if let Err(err) = self.client.start() {
                glue::log_error(&format!("Client error: {:?}", err));
            }
        });
    }
}

pub mod c {
    use std::os::raw::{c_char, c_void};
    use std::ffi::CStr;
    use std::ptr;

    use super::DiscordClient;
    use ::handler::Handler;
    use ::glue;

    #[no_mangle]
    pub extern "C" fn create_client(plugin: *const c_void, token: *const c_char) -> *mut c_void {
        let handler = Handler::new(plugin as usize);
        let c_str = unsafe { CStr::from_ptr(token).to_owned() };
         match c_str.to_str() {
            Ok(token) => {
                if let Some(client) = DiscordClient::new(token, handler) {
                    Box::into_raw(Box::new(client)) as *mut c_void
                } else {
                    ptr::null_mut()
                }
            },
            _ => {
                glue::log_error("Invalid discord token provided");
                ptr::null_mut()
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
    pub extern "C" fn client_set_ready_callback(client: *mut c_void, callback: *mut c_void) {
        unsafe {
            let client = &mut *(client as *mut DiscordClient);
            client.set_ready_callback(callback as usize);
        }
    }

    #[no_mangle]
    pub extern "C" fn connect_client(client: *mut c_void) {
        unsafe {
            let client = &mut *(client as *mut DiscordClient);
            client.start();
        }
    }
}