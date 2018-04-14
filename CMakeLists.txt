cmake_minimum_required(VERSION 3.5)
project(discord)

set(SOURCE_FILES
    cpp/Extension.cpp
    cpp/DiscordClient.cpp
    cpp/smn_client.cpp
    cpp/smn_message.cpp
    cpp/smn_ready.cpp
    cpp/include/smsdk_ext.cpp
    cpp/glue.cpp
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -DSOURCEMOD_BUILD -fno-strict-aliasing -m32")

include(ExternalProject)

ExternalProject_Add(
    discord_extension
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cargo build --target i686-unknown-linux-gnu COMMAND cargo build --release --target i686-unknown-linux-gnu
    BINARY_DIR "${CMAKE_SOURCE_DIR}/rust"
    INSTALL_COMMAND ""
)

include_directories(
    sourcemod/public
    sourcemod/public/amtl
    sourcemod/public/amtl/amtl
    sourcemod/sourcepawn/include
)

add_library(discord.ext SHARED ${SOURCE_FILES})

add_dependencies(discord.ext discord_extension)

target_link_libraries(discord.ext
    debug "-lpthread -ldl -lcrypto -lssl -Wl,--no-undefined -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-unknown-linux-gnu/debug/libdiscord_extension.a"
    optimized "-lpthread -ldl -lcrypto -lssl -Wl,--no-undefined -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-unknown-linux-gnu/release/libdiscord_extension.a"
)


set_target_properties(discord.ext PROPERTIES PREFIX "")