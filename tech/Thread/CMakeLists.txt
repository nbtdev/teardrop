# library name
set(TARGET Thread)

# library source files
set(SOURCES
    Task.cpp
    Task.h
    TaskGraph.cpp
    TaskGraph.h
    TaskProduct.cpp
    TaskProduct.h
    TaskProductManager.h
    TaskScheduler.cpp
    TaskScheduler.h
    TaskTypes.h
    ThreadUtil.h

    scheduler/Serial/SerialTaskScheduler.cpp
    scheduler/Serial/SerialTaskScheduler.h
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND SOURCES
        platform/Win32/ThreadUtil.cpp
    )
else()
    list(APPEND SOURCES
        platform/POSIX/ThreadUtil.cpp
    )

    list(APPEND PLATFORM_DEPENDENCIES
        pthread
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
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler
)
