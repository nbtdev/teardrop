# library name
set(TARGET Physics)

# library source files
set(SOURCES
    Body.cpp
    Body.h
    CharacterProxy.cpp
    CharacterProxy.h
    CollisionFilter.cpp
    CollisionFilter.h
    Physics.h
    PhysicsMemory.h
    Shape.cpp
    Shape.h
    World.cpp
    World.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
