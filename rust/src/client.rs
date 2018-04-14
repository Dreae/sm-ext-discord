use std::sync::Mutex;
use std::thread;

use typemap::Key;
use serenity::client::Client;
use ::handler::Handler;
use ::glue;

#[derive(Default)]
pub struct ClientCallbacks {
    pub msg_callback: Option<Mutex<usize>>
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

    pub fn start(&'static mut self) {
        thread::spawn(move || {
            if let Err(err) = self.client.start() {
                glue::log_error(&format!("Client error: {:?}", err));
            }
        });
    }
}