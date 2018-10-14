# library name
set(TARGET XWindow)

# library source files
set(SOURCES
    InputXWindow.cpp
    InputXWindow.h
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
        Core
)
