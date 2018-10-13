# library name
set(TARGET Stream)

# library source files
set(SOURCES
    FileStream.h
    MemoryStream.cpp
    MemoryStream.h
    Stream.cpp
    Stream.h
    TextReader.cpp
    TextReader.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
