use std::os::raw::{c_char, c_uchar};

#[repr(C)]
pub struct DiscordMessage {
    pub content: *const c_char,
    pub channel_id: u64,
    pub author_id: u64,
    pub bot: c_uchar,
    pub own: c_uchar,
    pub mentioned_roles: *const u64,
    pub num_mentioned_roles: u32,
    pub mentioned_users: *const u64,
    pub num_mentioned_users: u32
}
