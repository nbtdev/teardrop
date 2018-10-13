# library name
set(TARGET Ragdoll)

# library source files
set(SOURCES
    Ragdoll.cpp
    Ragdoll.h
    RagdollController.cpp
    RagdollController.h
    RagdollSystem.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
