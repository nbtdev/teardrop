# Required variables
if(NOT FREEIMAGE_DIR)
    set(FREEIMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools/ThirdParty/FreeImage)
endif()

if(NOT QT5_DIR)
    message(FATAL_ERROR "QT5_DIR must be provided")
endif()

if(NOT FBX_SDK_DIR)
    message(FATAL_ERROR "FBX_SDK_DIR must be provided")
endif()

if(NOT TBB_DIR)
    message(FATAL_ERROR "TBB_DIR must be provided")
endif()

cmake_minimum_required(VERSION 3.0)

enable_language(CXX)

# allow version info in project
cmake_policy(SET CMP0048 NEW)

# allow IN_LIST operator
cmake_policy(SET CMP0057 NEW)

project(Teardrop
    VERSION 2018.12
)

#
# global settings
#
# * use C++14
set(CMAKE_CXX_STANDARD 14)

#if(CMAKE_INSTALL_PREFIX)
#    set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
#else()
    set(TEARDROP_INSTALL_DIR ${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE})
#endif()

# TODO: we need to know if the caller provided CMAKE_INSTALL_PREFIX on the command line...
set(CMAKE_INSTALL_PREFIX ${TEARDROP_INSTALL_DIR})


# include build helpers
include(cmake/td-targets.cmake)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH x64)
else()
    set(ARCH x86)
endif()

message(STATUS "Arch: ${ARCH}")
message(STATUS "Using FreeImage located in ${FREEIMAGE_DIR}")
message(STATUS "Using Qt5 located in ${QT5_DIR}")
message(STATUS "Using FBX SDK located in ${FBX_SDK_DIR}")

add_subdirectory(tech)
add_subdirectory(tools)
add_subdirectory(integrations)
