use std::os::raw::{c_char, c_uchar};
use std::ffi::CString;

#[repr(C)]
pub struct DiscordMessage {
    pub content: *mut c_char,
    pub channel_id: u64,
    pub author_id: u64,
    pub bot: c_uchar
}


#[no_mangle]
pub extern "C" fn free_discord_message(msg: *mut DiscordMessage) {
    unsafe {
        let msg_ref = &*msg;

        CString::from_raw(msg_ref.content);
        Box::from_raw(msg);
    }
}