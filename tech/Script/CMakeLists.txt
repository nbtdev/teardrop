# library name
set(TARGET Script)

# library source files
set(SOURCES
    ScriptVM.cpp
    ScriptVM.h
    Variant.cpp
    Variant.h

    # TODO: integrations are being moved out of the SDK core
    integration/Lua/LuaScriptVM.cpp
    integration/Lua/LuaScriptVM.h
    integration/Lua/ScriptHelper.cpp
    integration/Lua/ScriptHelper.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)

target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/integration
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/Lua/src
)
