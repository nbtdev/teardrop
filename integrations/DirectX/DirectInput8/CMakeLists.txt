# library name
set(TARGET DirectInput8)

# library source files
set(SOURCES
    InputDI8.cpp
    InputDI8.h
    Integration.cpp
    Integration.h
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
        Gfx

        dinput
        tbb
)
