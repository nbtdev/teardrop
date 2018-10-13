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
    ThreadUtil.cpp
    ThreadUtil.h

    scheduler/Serial/SerialTaskScheduler.cpp
    scheduler/Serial/SerialTaskScheduler.h
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
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler
)
