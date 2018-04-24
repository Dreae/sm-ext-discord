pub mod c {
    use std::os::raw::{c_char, c_uchar};

    use serenity::model::channel::Message;
    use serenity::model::id::{ChannelId, UserId, GuildId, RoleId};

    use libc;
    use ::utils;

    #[no_mangle]
    pub extern "C" fn get_message_content(msg: *mut Message, buffer: *mut c_char, len: libc::size_t) {
        let msg = unsafe { &*msg };
        utils::strncpy(buffer, &msg.content, len);
    }

    #[no_mangle]
    pub extern "C" fn get_message_channel_id(msg: *mut Message) -> u64 {
        let msg = unsafe { &*msg };
        let ChannelId(id) = msg.channel_id;

        id
    }

    #[no_mangle]
    pub extern "C" fn get_message_author_id(msg: *mut Message) -> u64 {
        let msg = unsafe { &*msg };
        let UserId(id) = msg.author.id;

        id
    }

    #[no_mangle]
    pub extern "C" fn get_message_guild_id(msg: *mut Message) -> u64 {
        let msg = unsafe { &*msg };
        if let Some(GuildId(id)) = msg.guild_id() {
            id
        } else {
            0
        }
    }

    #[no_mangle]
    pub extern "C" fn get_message_is_self(msg: *mut Message) -> c_uchar {
        let msg = unsafe { &*msg };
        if msg.is_own() {
            1
        } else {
            0
        }
    }

    #[no_mangle]
    pub extern "C" fn get_message_is_bot(msg: *mut Message) -> c_uchar {
        let msg = unsafe { &*msg };
        if msg.author.bot {
            1
        } else {
            0
        }
    }

    #[no_mangle]
    pub extern "C" fn get_message_mentions_role(msg: *mut Message, role_id: u64) -> c_uchar {
        let msg = unsafe { &*msg };
        for &RoleId(id) in msg.mention_roles.iter() {
            if id == role_id {
                return 1;
            }
        }

        0
    }

    #[no_mangle]
    pub extern "C" fn get_message_num_mentioned_roles(msg: *mut Message) -> u32 {
        let msg = unsafe { &*msg };
        msg.mention_roles.len() as u32
    }

    #[no_mangle]
    pub extern "C" fn get_message_mentioned_role(msg: *mut Message, which: u32) -> u64 {
        let msg = unsafe { &*msg };
        if let Some(&RoleId(id)) = msg.mention_roles.get(which as usize) {
            id
        } else {
            0
        }
    }

    #[no_mangle]
    pub extern "C" fn get_message_mentions_user(msg: *mut Message, user_id: u64) -> c_uchar {
        let msg = unsafe { &*msg };
        for user in msg.mentions.iter() {
            let UserId(id) = user.id;
            if id == user_id {
                return 1;
            }
        }

        0
    }

    #[no_mangle]
    pub extern "C" fn get_message_num_mentioned_users(msg: *mut Message) -> u32 {
        let msg = unsafe { &*msg };
        msg.mentions.len() as u32
    }

    #[no_mangle]
    pub extern "C" fn get_message_mentioned_user(msg: *mut Message, which: u32) -> u64 {
        let msg = unsafe { &*msg };
        if let Some(user) = msg.mentions.get(which as usize) {
            let UserId(id) = user.id;
            id
        } else {
            0
        }
    }

    #[no_mangle]
    pub extern "C" fn free_discord_message(msg: *mut Message) {
        unsafe {
            Box::from_raw(msg);
        }
    }
}