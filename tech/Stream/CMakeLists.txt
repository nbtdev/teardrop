# library name
set(TARGET Stream)

# library source files
set(SOURCES
    FileStream.cpp
    FileStream.h
    MemoryStream.cpp
    MemoryStream.h
    Stream.cpp
    Stream.h
    TextReader.cpp
    TextReader.h
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND SOURCES
        Win32/FileStream.cpp
    )
else()
    list(APPEND SOURCES
        POSIX/FileStream.cpp
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
        Memory
        Util
)
