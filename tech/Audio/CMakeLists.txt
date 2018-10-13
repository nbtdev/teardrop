# library name
set(TARGET Audio)

# library source files
set(SOURCES
    Emitter.cpp
    Emitter.h
    Renderer.cpp
    Renderer.h
    Sample.cpp
    Sample.h
    Stream.cpp
    Stream.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
