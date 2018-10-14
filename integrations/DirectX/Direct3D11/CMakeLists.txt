# library name
set(TARGET Direct3D11)

# library source files
set(SOURCES
    BufferManagerD3D11.cpp
    BufferManagerD3D11.h
    FragmentShaderD3D11.cpp
    FragmentShaderD3D11.h
    IndexBufferD3D11.cpp
    IndexBufferD3D11.h
    Integration.h
    RefObj.h
    RendererD3D11.cpp
    RendererD3D11.h
    RenderTargetD3D11.cpp
    RenderTargetD3D11.h
    RenderTextureD3D11.cpp
    RenderTextureD3D11.h
    RenderWindowD3D11.cpp
    RenderWindowD3D11.h
    ShaderManagerD3D11.cpp
    ShaderManagerD3D11.h
    ShaderRealizerD3D11.cpp
    ShaderRealizerD3D11.h
    Texture2DD3D11.cpp
    Texture2DD3D11.h
    TextureManagerD3D11.cpp
    TextureManagerD3D11.h
    VertexBufferD3D11.cpp
    VertexBufferD3D11.h
    VertexDeclarationD3D11.cpp
    VertexDeclarationD3D11.h
    VertexShaderD3D11.cpp
    VertexShaderD3D11.h
    ViewportD3D11.cpp
    ViewportD3D11.h
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

        d3d11
        dxgi
        d3dcompiler
)
