# library name
set(TARGET Asset)

# library source files
set(SOURCES
    Asset.cpp
    Asset.h
    AttributeMapAsset.cpp
    AttributeMapAsset.h
    HeightfieldAsset.cpp
    HeightfieldAsset.h
    LandscapeAsset.cpp
    LandscapeAsset.h
    StaticMeshAsset.cpp
    StaticMeshAsset.h
    TextureAsset.cpp
    TextureAsset.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
