# library name
set(TARGET Reflection)

# library source files
set(SOURCES
    ClassDef.cpp
    ClassDef.h
    EnumDef.cpp
    EnumDef.h
    Object.cpp
    Object.h
    Property.h
    PropertyDef.cpp
    PropertyDef.h
    Reflection.h
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
        Memory
)
