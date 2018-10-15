if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # import the FreeImage library
    add_library(FreeImage SHARED IMPORTED)
    set(FREEIMAGE_LIB_DIR ${FREEIMAGE_DIR}/Dist/${ARCH})
    set(FREEIMAGE_INCLUDE_DIR ${FREEIMAGE_DIR}/Dist/${ARCH})

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        message(STATUS "Creating imported FreeImage target - files in ${FREEIMAGE_LIB_DIR}")
        set_target_properties(FreeImage PROPERTIES
            IMPORTED_LOCATION_DEBUG "${FREEIMAGE_LIB_DIR}/FreeImaged.dll"
            IMPORTED_LOCATION_RELEASE "${FREEIMAGE_LIB_DIR}/FreeImage.dll"
            IMPORTED_LOCATION_RELWITHDEBINFO "${FREEIMAGE_LIB_DIR}/FreeImage.dll"
            IMPORTED_IMPLIB_DEBUG "${FREEIMAGE_LIB_DIR}/FreeImaged.lib"
            IMPORTED_IMPLIB_RELEASE "${FREEIMAGE_LIB_DIR}/FreeImage.lib"
            IMPORTED_IMPLIB_RELWITHDEBINFO "${FREEIMAGE_LIB_DIR}/FreeImage.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${FREEIMAGE_INCLUDE_DIR}"
        )
    endif()

    install(FILES
        $<TARGET_FILE:FreeImage> DESTINATION ${TEARDROP_INSTALL_DIR}/bin
    )

    set(FREEIMAGE_TARGET FreeImage)
else()
    set(FREEIMAGE_TARGET freeimage)
endif()

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

# import the TBB library
add_library(TBB SHARED IMPORTED)

if(${ARCH} MATCHES "x64")
    set(TBB_ARCH intel64)
else()
    set(TBB_ARCH ia32)
endif()

set(TBB_INCLUDE_DIR ${TBB_DIR}/include)
message(STATUS "Creating imported TBB target - files in ${TBB_LIB_DIR}")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(TBB_LIB_DIR ${TBB_DIR}/lib/${TBB_ARCH}/vc14)
    set(TBB_BIN_DIR ${TBB_DIR}/bin/${TBB_ARCH}/vc14)

    set_target_properties(TBB PROPERTIES
        IMPORTED_LOCATION_RELEASE ${TBB_BIN_DIR}/tbb.dll
        IMPORTED_LOCATION_RELWITHDEBINFO ${TBB_BIN_DIR}/tbb.dll
        IMPORTED_LOCATION_DEBUG ${TBB_BIN_DIR}/tbb_debug.dll
        IMPORTED_IMPLIB_RELEASE ${TBB_LIB_DIR}/tbb.lib
        IMPORTED_IMPLIB_RELWITHDEBINFO ${TBB_LIB_DIR}/tbb.lib
        IMPORTED_IMPLIB_DEBUG ${TBB_LIB_DIR}/tbb_debug.lib
        INTERFACE_INCLUDE_DIRECTORIES ${TBB_INCLUDE_DIR}
    )
else()
    set(TBB_LIB_DIR ${TBB_DIR}/lib/${TBB_ARCH}/gcc4.7)
    set(TBB_BIN_DIR ${TBB_DIR}/bin/${TBB_ARCH}/gcc4.7)

    set_target_properties(TBB PROPERTIES
        IMPORTED_LOCATION_RELEASE ${TBB_LIB_DIR}/libtbb.so.2
        IMPORTED_LOCATION_REL ${TBB_LIB_DIR}/libtbb.so.2
        IMPORTED_LOCATION_DEBUG ${TBB_LIB_DIR}/libtbb_debug.so.2
        INTERFACE_LINK_OPTIONS -L${TBB_LIB_DIR}
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

    set(ZLIB zlib)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
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
        ${FREEIMAGE_TARGET}
        FBX-SDK
        LibHFZ
        TBB
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
