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
        platform/Win32/FileSystemWatcherWin32.cpp
        platform/Win32/FileSystemWatcherWin32.h
        platform/Win32/FileSystemWin32.cpp
        platform/Win32/TimerWin32.cpp
        platform/Win32/UUID.cpp
    )

    list(APPEND PLATFORM_DEPENDENCIES
        Shlwapi
    )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND SOURCES
        platform/POSIX/FileSystem.cpp
        platform/POSIX/Timer.cpp
        platform/POSIX/UUID.cpp
    )
endif()

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)

target_link_libraries(${TARGET}
    PUBLIC
        ${PLATFORM_DEPENDENCIES}
)

target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
)
