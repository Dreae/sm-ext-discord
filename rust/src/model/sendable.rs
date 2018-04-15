use std::collections::HashMap;

use serenity::model::id::ChannelId;

#[derive(Default)]
pub struct SendableDiscordMessage {
    pub content: Option<String>,
    pub embed: Option<SendableDiscordEmbed>
}

#[derive(Default)]
pub struct SendableDiscordEmbed {
    pub description: Option<String>,
    pub title: Option<String>,
    pub fields: HashMap<String, (String, bool)>,
    pub color: Option<(u8, u8, u8)>,
    pub footer: Option<String>
}

impl SendableDiscordMessage {
    pub fn new() -> SendableDiscordMessage {
        Default::default()
    }

    pub fn send(self, channel_id: ChannelId) {
        if let Err(err) = channel_id.send_message(|m| {
            let mut m = m;
            if let Some(content) = self.content {
                m = m.content(content);
            }

            if let Some(embed) = self.embed {
                m = m.embed(|e| {
                    let mut e = e;
                    if let Some(description) = embed.description {
                        e = e.description(description);
                    }

                    if let Some(color) = embed.color {
                        e = e.color(color);
                    }

                    if let Some(title) = embed.title {
                        e = e.title(title);
                    }

                    for (k, v) in embed.fields.into_iter() {
                        e = e.field(k, v.0, v.1);
                    }

                    if let Some(footer) = embed.footer {
                        e = e.footer(|f| f.text(footer));
                    }

                    e
                });
            }

            m
        }) {
            ::glue::log_error(&format!("Error sending message: {:?}", err));
        }
    }
}

pub mod c {
    use super::*;
    use std::os::raw::{c_char, c_uchar};
    use std::ffi::CStr;

    use serenity::model::id::ChannelId;

    #[no_mangle]
    pub extern "C" fn create_new_discord_message() -> *mut SendableDiscordMessage {
        Box::into_raw(Box::new(SendableDiscordMessage::new()))
    }

    #[no_mangle]
    pub extern "C" fn set_new_message_content(new_message: *mut SendableDiscordMessage, content: *const c_char) {
        let new_message = unsafe { &mut *new_message };
        let c_str = unsafe { CStr::from_ptr(content).to_owned() };
        let content = c_str.into_string().unwrap_or_default();

        new_message.content = Some(content);
    }

    #[no_mangle]
    pub extern "C" fn set_new_message_embed(new_message: *mut SendableDiscordMessage, embed: *mut SendableDiscordEmbed) {
        let new_message = unsafe { &mut *new_message };
        let embed = unsafe { Box::from_raw(embed) };

        new_message.embed = Some(*embed);
    }

    #[no_mangle]
    pub extern "C" fn send_new_discord_message(channel_id: u64, new_message: *mut SendableDiscordMessage) {
        let new_message = unsafe { Box::from_raw(new_message) };
        new_message.send(ChannelId(channel_id));
    }

    #[no_mangle]
    pub extern "C" fn create_new_discord_embed() -> *mut SendableDiscordEmbed {
        Box::into_raw(Box::new(Default::default()))
    }

    #[no_mangle]
    pub extern "C" fn set_new_embed_title(new_embed: *mut SendableDiscordEmbed, title: *const c_char) {
        let new_embed = unsafe { &mut *new_embed };
        let c_str = unsafe { CStr::from_ptr(title).to_owned() };
        let title = c_str.into_string().unwrap_or_default();

        new_embed.title = Some(title);
    }

    #[no_mangle]
    pub extern "C" fn set_new_embed_description(new_embed: *mut SendableDiscordEmbed, description: *const c_char) {
        let new_embed = unsafe { &mut *new_embed };
        let c_str = unsafe { CStr::from_ptr(description).to_owned() };
        let description = c_str.into_string().unwrap_or_default();

        new_embed.description = Some(description);
    }

    #[no_mangle]
    pub extern "C" fn new_embed_add_field(new_embed: *mut SendableDiscordEmbed, title: *const c_char, value: *const c_char, inline: c_uchar) {
        let new_embed = unsafe { &mut *new_embed };
        let c_str = unsafe { CStr::from_ptr(title).to_owned() };
        let title = c_str.into_string().unwrap_or_default();

        let c_str = unsafe { CStr::from_ptr(value).to_owned() };
        let value = c_str.into_string().unwrap_or_default();

        let inline = if inline == 1 { true } else { false };

        new_embed.fields.insert(title, (value, inline));
    }
}