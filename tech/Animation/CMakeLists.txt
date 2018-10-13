# library name
set(TARGET Animation)

# library source files
set(SOURCES
    Animation.cpp
    Animation.h
    AnimationBlender.cpp
    AnimationBlender.h
    BlendGraph.cpp
    BlendGraph.h
    Bone.cpp
    Bone.h
    Rig.cpp
    Rig.h
    SkeletonInstance.cpp
    SkeletonInstance.h
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
        Math
        Memory
        Util
)
