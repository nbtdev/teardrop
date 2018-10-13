# library name
set(TARGET Math)

# library source files
set(SOURCES
    AABB.cpp
    AABB.h
    Intersection.h
    MathUtil.cpp
    MathUtil.h
    Matrix44.cpp
    Matrix44.h
    MT.cpp
    Plane.cpp
    Plane.h
    Quaternion.cpp
    Quaternion.h
    Query.cpp
    Query.h
    Ray.cpp
    Ray.h
    Sphere.cpp
    Sphere.h
    Transform.cpp
    Transform.h
    Triangle.cpp
    Triangle.h
    Vector2.cpp
    Vector2.h
    Vector4.cpp
    Vector4.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
