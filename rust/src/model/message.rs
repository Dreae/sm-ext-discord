use std::os::raw::{c_char, c_uchar};

#[repr(C)]
pub struct DiscordMessage {
    pub content: *mut c_char,
    pub channel_id: u64,
    pub author_id: u64,
    pub bot: c_uchar,
    pub own: c_uchar
}
