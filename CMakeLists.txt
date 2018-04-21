cmake_minimum_required(VERSION 3.5)
project(discord)

if(NOT SDK)
    set(SDK csgo)
endif()

set(SOURCE_FILES
    cpp/Extension.cpp
    cpp/smn_extension.cpp
    cpp/smn_message.cpp
    cpp/smn_newmessage.cpp
    cpp/smn_newembed.cpp
    cpp/smn_ready.cpp
    cpp/smn_user.cpp
    cpp/include/smsdk_ext.cpp
    cpp/glue.cpp
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DSOURCEMOD_BUILD -DSMEXT_CONF_METAMOD)

if(MSVC)
    add_definitions(-DWIN32 -D_WINDOWS -DCOMPILER_MSVC -DCOMPILER_MSVC32)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fno-strict-aliasing -m32")
    add_definitions(-D_LINUX -DPOSIX)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-DCOMPILER_GCC -D_vsnprintf=vsnprintf -D_snprintf=snprintf -D_stricmp=strcasecmp -Dstricmp=strcasecmp)
endif()

if(${SDK} STREQUAL "insurgency")
    add_definitions(-DNETWORK_VARS_ENABLED)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
    endif()
endif()

include(CMakeLists.rust.txt)

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

if(MSVC)
    link_directories(
        hl2sdk/lib/public
    )
else()
    link_directories(
        hl2sdk/lib/linux
    )
endif()

add_library(discord.ext SHARED ${SOURCE_FILES})

add_dependencies(discord.ext discord_extension)

if(MSVC)
    target_link_libraries(discord.ext
        general kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib legacy_stdio_definitions.lib userenv.lib ncrypt.lib secur32.lib ws2_32.lib crypt32.lib tier1.lib tier0.lib
        debug discord_rust
        optimized discord_rust
    )

    if(${SDK} STREQUAL "csgo" OR ${SDK} STREQUAL "insurgency")
        target_link_libraries(discord.ext general interfaces.lib)
    endif()
else()
    target_link_libraries(discord.ext
        general "-Wl,--no-undefined -lpthread -ldl -m32 -static-libgcc -static-libstdc++"
        debug discord_rust
        optimized discord_rust
    )

    target_link_libraries(discord.ext general :libssl.a :libcrypto.a :libz.a :tier1_i486.a)

    if(${SDK} STREQUAL "csgo")
        target_link_libraries(discord.ext general :interfaces_i486.a tier0)
    elseif(${SDK} STREQUAL "insurgency")
        target_link_libraries(discord.ext general :interfaces_i486.a tier0_srv)
    else()
        target_link_libraries(discord.ext general tier0_srv)
    endif()
endif()

set_target_properties(discord.ext PROPERTIES PREFIX "")
