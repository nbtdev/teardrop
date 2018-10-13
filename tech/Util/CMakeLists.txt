# library name
set(TARGET Util)

# library source files
set(SOURCES
    Environment.cpp
    Environment.h
    Event.h
    FileSystem.cpp
    FileSystem.h
    FileSystemWatcher.cpp
    FileSystemWatcher.h
    Hash.cpp
    Hash.h
    Logger.cpp
    Logger.h
    RefCounted.h
    SharedPointer.h
    String.cpp
    StringUtil.cpp
    StringUtil.h
    System.h
    SystemManager.cpp
    SystemManager.h
    Timer.h
    UUID.h
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND SOURCES
        platform/WIN32/FileSystemWatcherWin32.cpp
        platform/WIN32/FileSystemWatcherWin32.h
        platform/WIN32/TimerWin32.cpp
        platform/WIN32/UUID.cpp
    )
endif()

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)

target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
)
