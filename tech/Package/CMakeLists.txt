# library name
set(TARGET Package)

# library source files
set(SOURCES
    DeferredResolution.h
    Executable.cpp
    Executable.h
    Logic.cpp
    Logic.h
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

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)

target_link_libraries(${TARGET}
    PUBLIC
        Util
)
