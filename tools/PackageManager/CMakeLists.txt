# import the FBX SDK library
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(FBX_COMPILER vs2015)
    set(FBX_LIB_NAME libfbxsdk-md.lib)
else()
    set(FBX_COMPILER gcc4)
    set(FBX_LIB_NAME libfbxsdk.a)
endif()

add_library(FBX-SDK STATIC IMPORTED)
string(TOLOWER ${CMAKE_BUILD_TYPE} FBX_BUILD_TYPE)

if(${FBX_BUILD_TYPE} MATCHES "relwithdebinfo")
    set(FBX_BUILD_TYPE "release")
endif()

set(FBX_SDK_LIB_DIR ${FBX_SDK_DIR}/lib/${FBX_COMPILER}/${ARCH}/${FBX_BUILD_TYPE})
set(FBX_SDK_INCLUDE_DIR ${FBX_SDK_DIR}/include)

message(STATUS "Creating imported FBX SDK target - files in ${FBX_SDK_LIB_DIR}")
set_target_properties(FBX-SDK PROPERTIES
    IMPORTED_LOCATION ${FBX_SDK_LIB_DIR}/${FBX_LIB_NAME}
    INTERFACE_INCLUDE_DIRECTORIES ${FBX_SDK_INCLUDE_DIR}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_target_properties(FBX-SDK PROPERTIES
        INTERFACE_LINK_LIBRARIES "pthread;dl"
    )
endif()

find_package(TBB REQUIRED PATHS ${TBB_DIR}/cmake)
if(TBB_FOUND)
    message(STATUS "TBB found: ${TBB_IMPORTED_TARGETS}")
    foreach(TBB_TARGET ${TBB_IMPORTED_TARGETS})
        get_target_property(TBB_FILE ${TBB_TARGET} LOCATION_${CMAKE_BUILD_TYPE})
        list(APPEND TBB_FILES ${TBB_FILE})
    endforeach()

    install(FILES ${TBB_FILES} DESTINATION ${TEARDROP_INSTALL_DIR}/bin)
else()
    message(FATAL_ERROR "TBB not found (TBB_DIR='${TBB_DIR}')")
endif()

# library name
set(TARGET PackageManager)

# library source files
set(SOURCES
    AssetImport.h
    AssetImportException.cpp
    AssetImportException.h
    AssetMetadata.cpp
    AssetMetadata.h
    AttributeMapAssetMetadata.cpp
    AttributeMapAssetMetadata.h
    Folder.cpp
    Folder.h
    HeightfieldAssetMetadata.cpp
    HeightfieldAssetMetadata.h
    ImportedAsset.cpp
    ImportedAsset.h
    ImportStaticMesh.cpp
    LandscapeAssetMetadata.cpp
    LandscapeAssetMetadata.h
    LandscapeImport.cpp
    Metadata.cpp
    Metadata.h
    ObjectMetadata.cpp
    ObjectMetadata.h
    PackageManager.cpp
    PackageManager.h
    PackageMetadata.cpp
    PackageMetadata.h
    TextureAssetMetadata.cpp
    TextureAssetMetadata.h
    TextureImport.cpp
    Thumbnail.cpp
    Thumbnail.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND PLATFORM_DEPENDENCIES
        Rpcrt4
    )

    set(ZLIB zlib)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND PLATFORM_DEPENDENCIES
        GLEW
    )

    set(ZLIB z)
endif()

target_link_libraries(${TARGET}
    PUBLIC
        Asset
        Core
        Package
        Reflection
        Stream
        Util

        # imported targets
        FreeImage
        FBX-SDK
        LibHFZ
        TBB::tbb
        TBB::tbbmalloc
        squish
        ${ZLIB}

        ${PLATFORM_DEPENDENCIES}

    PRIVATE
        tinyxml
)

# workaround -- squish doesn't set include directories for its target,
# so we'll have to do it here
target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/squish
)
