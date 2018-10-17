# library name
set(TARGET Package)

# library source files
set(SOURCES
    DeferredResolution.h
    Package.cpp
    Package.h
    PackageMetadataSerializer.cpp
    PackageMetadataSerializer.h
    PackageSerializer.cpp
    PackageSerializer.h
    UUIDReader.cpp
    UUIDReader.h
    UUIDStreamReader.h
    UUIDStreamWriter.h
    UUIDWriter.cpp
    UUIDWriter.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND SOURCES
        platform/Win32/UUIDStreamReader.cpp
        platform/Win32/UUIDStreamWriter.cpp
    )

    list(APPEND PLATFORM_DEPENDENCIES
        Ws2_32
    )
else()
    list(APPEND SOURCES
        platform/POSIX/UUIDStreamReader.cpp
        platform/POSIX/UUIDStreamWriter.cpp
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
        Asset
        Core
        Memory
        Reflection
        Stream
        Util

        ${PLATFORM_DEPENDENCIES}

    PRIVATE
        tinyxml
)
