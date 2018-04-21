cmake_minimum_required(VERSION 3.5)
project(discord)

set(SOURCE_FILES
    cpp/Extension.cpp
    cpp/smn_extension.cpp
    cpp/smn_message.cpp
    cpp/smn_newmessage.cpp
    cpp/smn_newembed.cpp
    cpp/smn_ready.cpp
    cpp/include/smsdk_ext.cpp
    cpp/glue.cpp
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -DSOURCEMOD_BUILD -D_LINUX -DPOSIX -DCOMPILER_GCC -DSMEXT_CONF_METAMOD -fno-strict-aliasing -m32")

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
    metamod-source/core
    metamod-source/core/sourcehook
    hl2sdk/game/shared
    hl2sdk/public/vstdlib
    hl2sdk/public/tier1
    hl2sdk/public/tier0
    hl2sdk/public/engine
    hl2sdk/public
    hl2sdk/dlls
)

link_directories(
    hl2sdk/lib/linux
)

add_library(discord.ext SHARED ${SOURCE_FILES})

add_dependencies(discord.ext discord_extension)

target_link_libraries(discord.ext
    debug "-Wl,--no-undefined -ltier0 -lpthread -ldl -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-unknown-linux-gnu/debug/libdiscord_extension.a -l:libssl.a -l:libcrypto.a -l:libz.a -l:tier1_i486.a -l:interfaces_i486.a"
    optimized "-Wl,--no-undefined -ltier0 -lpthread -ldl -m32 -static-libgcc -static-libstdc++ ${CMAKE_SOURCE_DIR}/rust/target/i686-unknown-linux-gnu/release/libdiscord_extension.a -l:libssl.a -l:libcrypto.a -l:libz.a -l:tier1_i486.a -l:interfaces_i486.a"
)


set_target_properties(discord.ext PROPERTIES PREFIX "")
