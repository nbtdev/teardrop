# library name
set(TARGET Core)

# library source files
set(SOURCES
    Input.cpp
    Input.h
    InputEvent.h
    Integration.cpp
    Integration.h
    Singleton.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
