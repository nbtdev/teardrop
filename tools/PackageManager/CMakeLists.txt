# import the FreeImage library
add_library(FreeImage SHARED IMPORTED)
set(FREEIMAGE_LIB_DIR ${FREEIMAGE_DIR}/Dist/${ARCH})
set(FREEIMAGE_INCLUDE_DIR ${FREEIMAGE_DIR}/Dist/${ARCH})

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "Creating imported FreeImage target - files in ${FREEIMAGE_LIB_DIR}")
    set_target_properties(FreeImage PROPERTIES
        IMPORTED_LOCATION_DEBUG "${FREEIMAGE_LIB_DIR}/FreeImaged.dll"
        IMPORTED_LOCATION_RELEASE "${FREEIMAGE_LIB_DIR}/FreeImage.dll"
        IMPORTED_IMPLIB_DEBUG "${FREEIMAGE_LIB_DIR}/FreeImaged.lib"
        IMPORTED_IMPLIB_RELEASE "${FREEIMAGE_LIB_DIR}/FreeImage.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${FREEIMAGE_INCLUDE_DIR}"
    )
endif()

install(FILES
    $<TARGET_FILE:FreeImage> DESTINATION ${TEARDROP_INSTALL_DIR}/bin
)

# import the FBX SDK library
add_library(FBX-SDK STATIC IMPORTED)
string(TOLOWER ${CMAKE_BUILD_TYPE} FBX_BUILD_TYPE)
set(FBX_SDK_LIB_DIR ${FBX_SDK_DIR}/lib/vs2015/${ARCH}/${FBX_BUILD_TYPE})
set(FBX_SDK_INCLUDE_DIR ${FBX_SDK_DIR}/include)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "Creating imported FBX SDK target - files in ${FBX_SDK_LIB_DIR}")
    set_target_properties(FBX-SDK PROPERTIES
        IMPORTED_LOCATION ${FBX_SDK_LIB_DIR}/libfbxsdk-md.lib
        INTERFACE_INCLUDE_DIRECTORIES ${FBX_SDK_INCLUDE_DIR}
    )
endif()

# import the TBB library
add_library(TBB SHARED IMPORTED)

if(${ARCH} MATCHES "x64")
    set(TBB_ARCH intel64)
else()
    set(TBB_ARCH ia32)
endif()

set(TBB_LIB_DIR ${TBB_DIR}/lib/${TBB_ARCH}/vc14)
set(TBB_BIN_DIR ${TBB_DIR}/bin/${TBB_ARCH}/vc14)
set(TBB_INCLUDE_DIR ${TBB_DIR}/include)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "Creating imported TBB target - files in ${TBB_LIB_DIR}")
    set_target_properties(TBB PROPERTIES
        IMPORTED_LOCATION_RELEASE ${TBB_BIN_DIR}/tbb.dll
        IMPORTED_LOCATION_DEBUG ${TBB_BIN_DIR}/tbb_debug.dll
        IMPORTED_IMPLIB_RELEASE ${TBB_LIB_DIR}/tbb.lib
        IMPORTED_IMPLIB_DEBUG ${TBB_LIB_DIR}/tbb_debug.lib
        INTERFACE_INCLUDE_DIRECTORIES ${TBB_INCLUDE_DIR}
    )
endif()

install(FILES
    $<TARGET_FILE:TBB> DESTINATION ${TEARDROP_INSTALL_DIR}/bin
)

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
        TBB
        squish
        zlib

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
