# library name
set(TARGET OpenGL)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # on Windows, we need to build the embedded Glew lib (we can install the dev
    # package on Linux)
    add_subdirectory(extern/glew/build/cmake)
    list(APPEND PLATFORM_DEPENDENCIES
        glew
    )

    list(APPEND PLATFORM_SOURCES
        platform/Win32/RendererOpenGL_createRenderWindow.cpp
        platform/Win32/RenderWindowOpenGL.cpp
        platform/Win32/RenderWindowOpenGL.h
    )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND PLATFORM_SOURCES
        platform/GLX/RendererOpenGL_createRenderWindow.cpp
        platform/GLX/RenderWindowOpenGL.cpp
        platform/GLX/RenderWindowOpenGL.h
    )

    list(APPEND PLATFORM_DEPENDENCIES
        X11
        GL
        GLEW
    )
endif()

# library source files
set(SOURCES
    BufferManagerOpenGL.cpp
    BufferManagerOpenGL.h
    FragmentShaderOpenGL.cpp
    FragmentShaderOpenGL.h
    GLHeaders.h
    IndexBufferOpenGL.cpp
    IndexBufferOpenGL.h
    IntegrationOpenGL.cpp
    IntegrationOpenGL.h
    ProgramOpenGL.cpp
    ProgramOpenGL.h
    RendererOpenGL.cpp
    RendererOpenGL.h
    RenderTargetOpenGL.cpp
    RenderTargetOpenGL.h
    ShaderManagerOpenGL.cpp
    ShaderManagerOpenGL.h
    Texture2DOpenGL.cpp
    Texture2DOpenGL.h
    TextureManagerOpenGL.cpp
    TextureManagerOpenGL.h
    VertexBufferOpenGL.cpp
    VertexBufferOpenGL.h
    VertexDeclarationOpenGL.cpp
    VertexDeclarationOpenGL.h
    VertexShaderOpenGL.cpp
    VertexShaderOpenGL.h
    ViewportOpenGL.cpp
    ViewportOpenGL.h
    ${PLATFORM_SOURCES}
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
        Gfx

        ${PLATFORM_DEPENDENCIES}
)

# workaround - glew doesn't set an interface include path
target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/glew/include
)
