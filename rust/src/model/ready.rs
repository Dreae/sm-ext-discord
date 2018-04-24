

pub mod c {
    use serenity::model::gateway::Ready;
    use serenity::model::id::UserId;

    #[no_mangle]
    pub extern "C" fn get_ready_user_id(ready: *mut Ready) -> u64 {
        let ready = unsafe { &*ready };
        let UserId(id) = ready.user.id;
        id
    }

    #[no_mangle]
    pub extern "C" fn free_discord_ready(ready: *mut Ready) {
        unsafe {
            Box::from_raw(ready);
        }
    }
}