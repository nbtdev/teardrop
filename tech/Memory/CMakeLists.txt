# library name
set(TARGET Memory)

# library source files
set(SOURCES
    AllocationTracker.cpp
    AllocationTracker.h
    Allocators.cpp
    Allocators.h
    malloc-280.c
    malloc-280.h
    Memory.cpp
    Memory.h
    MemoryRegion.cpp
    MemoryRegion.h
    Pool.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set_source_files_properties(malloc-280.c PROPERTIES COMPILE_FLAGS "/wd4267 /wd4312")
endif()

target_link_libraries(${TARGET}
    PUBLIC
        Stream
)

target_compile_definitions(${TARGET}
    PRIVATE
        USE_DL_MALLOC
)
