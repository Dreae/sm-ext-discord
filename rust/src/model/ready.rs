#[repr(C)]
pub struct DiscordReady {
    pub user_id: u64
}

#[no_mangle]
pub extern "C" fn free_discord_ready(ready: *mut DiscordReady) {
    unsafe {
        Box::from_raw(ready);
    }
}