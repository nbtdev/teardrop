# library name
set(TARGET Core)

# library source files
set(SOURCES
    Executable.cpp
    Executable.h
    Input.cpp
    Input.h
    InputEvent.h
    Integration.cpp
    Integration.h
    Logic.cpp
    Logic.h
    Singleton.h
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
        Reflection
)
