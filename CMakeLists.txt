cmake_minimum_required(VERSION 3.5)
project(discord)

set(SOURCE_FILES
    cpp/Extension.cpp
    cpp/DiscordClient.cpp
    cpp/smn_extension.cpp
    cpp/smn_client.cpp
    cpp/smn_message.cpp
    cpp/smn_newmessage.cpp
    cpp/smn_newembed.cpp
    cpp/smn_ready.cpp
    cpp/include/smsdk_ext.cpp
    cpp/glue.cpp
)

if(NOT TARGET)
        set(TARGET linux)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -DSOURCEMOD_BUILD -fno-strict-aliasing -m32")

if(${TARGET} STREQUAL linux)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32 -D_STAT_DEFINED")
endif()

include(ExternalProject)

if(${TARGET} STREQUAL linux)
    ExternalProject_Add(
        discord_extension
        DOWNLOAD_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND cargo build --target i686-unknown-linux-gnu COMMAND cargo build --release --target i686-unknown-linux-gnu
        BINARY_DIR "${CMAKE_SOURCE_DIR}/rust"
        INSTALL_COMMAND ""
    )
else()
    ExternalProject_Add(
        discord_extension
        DOWNLOAD_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND RUSTFLAGS=-Cpanic=abort cargo build --target i686-pc-windows-gnu COMMAND RUSTFLAGS=-Cpanic=abort cargo build --release --target i686-pc-windows-gnu
        BINARY_DIR "${CMAKE_SOURCE_DIR}/rust"
        INSTALL_COMMAND ""
    )
endif()

include_directories(
    sourcemod/public
    sourcemod/public/amtl
    sourcemod/public/amtl/amtl
    sourcemod/sourcepawn/include
)

add_library(discord.ext SHARED ${SOURCE_FILES})

add_dependencies(discord.ext discord_extension)

if(${TARGET} STREQUAL linux)
    target_link_libraries(discord.ext
        debug "-Wl,--no-undefined -lpthread -ldl -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-unknown-linux-gnu/debug/libdiscord_extension.a -l:libssl.a -l:libcrypto.a -l:libz.a"
        optimized "-Wl,--no-undefined -lpthread -ldl -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-unknown-linux-gnu/release/libdiscord_extension.a -l:libssl.a -l:libcrypto.a -l:libz.a"
    )
else()
    target_link_libraries(discord.ext
        debug "-Wl,--no-undefined -lpthread -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-pc-windows-gnu/debug/discord_extension.lib -lncrypt -luserenv -lsecur32 -lcrypt32 -lz -lwsock32 -lgdi32 -lws2_32"
        optimized "-Wl,--no-undefined -lpthread -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-pc-windows-gnu/release/discord_extension.lib -lncrypt -luserenv -lsecur32 -lcrypt32 -lz -lwsock32 -lgdi32 -lws2_32"
    )
endif()

set_target_properties(discord.ext PROPERTIES PREFIX "")

if(NOT ${TARGET} STREQUAL linux)
        set_target_properties(discord.ext PROPERTIES SUFFIX ".dll")
endif()