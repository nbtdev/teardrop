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

# squish actually gives us a CMakeLists.txt we can use to build their target(s)
add_subdirectory(squish)
