# only need this for Windows; on Linux zlib is sensibly available easily via apt/yum
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    #
    # Build a target from the zlib source that comes with FreeImage
    #
    set(ZLIB_TARGET zlib)

    set(ZLIB_SOURCES
        FreeImage/Source/ZLib/adler32.c
        FreeImage/Source/ZLib/compress.c
        FreeImage/Source/ZLib/crc32.c
        FreeImage/Source/ZLib/crc32.h
        FreeImage/Source/ZLib/deflate.c
        FreeImage/Source/ZLib/deflate.h
        FreeImage/Source/ZLib/gzclose.c
        FreeImage/Source/ZLib/gzguts.h
        FreeImage/Source/ZLib/gzlib.c
        FreeImage/Source/ZLib/gzread.c
        FreeImage/Source/ZLib/gzwrite.c
        FreeImage/Source/ZLib/infback.c
        FreeImage/Source/ZLib/inffast.c
        FreeImage/Source/ZLib/inffast.h
        FreeImage/Source/ZLib/inffixed.h
        FreeImage/Source/ZLib/inflate.c
        FreeImage/Source/ZLib/inflate.h
        FreeImage/Source/ZLib/inftrees.c
        FreeImage/Source/ZLib/inftrees.h
        FreeImage/Source/ZLib/trees.c
        FreeImage/Source/ZLib/trees.h
        FreeImage/Source/ZLib/uncompr.c
        FreeImage/Source/ZLib/zconf.h
        FreeImage/Source/ZLib/zlib.h
        FreeImage/Source/ZLib/zutil.c
        FreeImage/Source/ZLib/zutil.h
    )

    add_Library(${ZLIB_TARGET}
        STATIC
        ${ZLIB_SOURCES}
    )

    target_include_directories(${ZLIB_TARGET}
        PUBLIC
            FreeImage/Source/ZLib
    )

    if (MSVC)
        target_compile_definitions(${ZLIB_TARGET} PRIVATE /W0)
    else()
        target_compile_definitions(${ZLIB_TARGET} PRIVATE -Wnone)
    endif()
endif()

#
# Build a target for LibHFZ
#
set(HFZ_TARGET LibHFZ)

set(HFZ_SOURCES
    LibHFZ/libhfz.cpp
    LibHFZ/libhfz.h
    LibHFZ/wrapper.cpp
    LibHFZ/wrapper.h
)

add_Library(${HFZ_TARGET}
    STATIC
    ${HFZ_SOURCES}
)

target_include_directories(${HFZ_TARGET}
    PUBLIC
        LibHFZ
)

target_compile_definitions(${HFZ_TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)

# squish actually gives us a CMakeLists.txt we can use to build their target(s)
add_subdirectory(squish EXCLUDE_FROM_ALL)

