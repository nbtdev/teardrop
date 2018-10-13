# library name
set(TARGET VFX)

# library source files
set(SOURCES
    Affector.cpp
    Affector.h
    Emitter.cpp
    Emitter.h
    ParticleData.h
    ParticleSystem.cpp
    ParticleSystem.h
    ParticleSystemRenderer.cpp
    ParticleSystemRenderer.h

    Emitters/PointEmitter.cpp
    Emitters/PointEmitter.h
    Emitters/RandomPointEmitter.cpp
    Emitters/RandomPointEmitter.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
