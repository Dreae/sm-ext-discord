pub mod c {
    use std::os::raw::{c_char, c_uchar, c_void};

    use serenity::model::user::User;
    use serenity::model::id::UserId;

    use libc;
    use ::utils;
    use ::THREADPOOL;

    #[no_mangle]
    pub extern "C" fn get_user(user_id: u64, callback: *const c_void, plugin: *const c_void, data: i32) {
        let user_id = UserId(user_id);

        let callback = callback as usize;
        let plugin = plugin as usize;

        THREADPOOL.spawn(move || {
            if let Ok(user) = user_id.get() {
                ::glue::call_user_callback(Box::new(user), callback, plugin, data);
            } else {
                ::glue::log_error("Error getting user from Discord API")
            }
        })
    }

    #[no_mangle]
    pub extern "C" fn free_discord_user(user: *mut User) {
        unsafe {
            Box::from_raw(user);
        }
    }

    #[no_mangle]
    pub extern "C" fn user_get_username(user: *mut User, buffer: *mut c_char, n: libc::size_t) {
        let user = unsafe { &mut *user };
        utils::strncpy(buffer, &user.name, n);
    }

    #[no_mangle]
    pub extern "C" fn user_get_tag(user: *mut User, buffer: *mut c_char, n: libc::size_t) {
        let user = unsafe { &mut *user };
        utils::strncpy(buffer, &user.tag(), n);
    }

    #[no_mangle]
    pub extern "C" fn user_has_role(user: *mut User, guild_id: u64, role_id: u64) -> c_uchar {
        let user = unsafe { &mut *user };

        if user.has_role(guild_id, role_id) { 1 } else { 0 }
    }
}