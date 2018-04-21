use std::thread;

use serenity::client::Client;
use ::handler::Handler;
use ::glue;

pub struct DiscordClient {
    client: Client
}

impl DiscordClient {
    pub fn new(token: &str, handler: Handler) -> Option<DiscordClient> {
        match Client::new(token, handler) {
            Ok(client) => {
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

    pub fn start(mut self) {
        thread::spawn(move || {
            if let Err(err) = self.client.start() {
                glue::log_error(&format!("Client error: {:?}", err));
            }
        });
    }
}
