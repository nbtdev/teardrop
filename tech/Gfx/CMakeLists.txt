# library name
set(TARGET Gfx)

# library source files
set(SOURCES
    AddColorExpression.cpp
    AddColorExpression.h
    Allocators.h
    Attribute.cpp
    Attribute.h
    BufferManager.cpp
    BufferManager.h
    Camera.cpp
    Camera.h
    Common.cpp
    Common.h
    Connection.cpp
    Connection.h
    Exception.cpp
    Exception.h
    FragmentShader.cpp
    FragmentShader.h
    IMeshInstanceProvider.h
    IndexBuffer.cpp
    IndexBuffer.h
    Light.cpp
    Light.h
    Material.cpp
    Material.h
    MaterialExpression.cpp
    MaterialExpression.h
    MaterialOutput.cpp
    MaterialOutput.h
    Mesh.cpp
    Mesh.h
    Renderable.h
    Renderer.cpp
    Renderer.h
    RenderTarget.cpp
    RenderTarget.h
    Sampler2D.cpp
    Sampler2D.h
    Sampler2DExpression.cpp
    Sampler2DExpression.h
    ShaderConstant.cpp
    ShaderConstant.h
    ShaderConstantTable.cpp
    ShaderConstantTable.h
    ShaderFeatures.cpp
    ShaderFeatures.h
    ShaderManager.cpp
    ShaderManager.h
    Submesh.cpp
    Submesh.h
    Texture2D.cpp
    Texture2D.h
    TextureManager.cpp
    TextureManager.h
    VertexBuffer.cpp
    VertexBuffer.h
    VertexDeclaration.cpp
    VertexDeclaration.h
    VertexElement.cpp
    VertexElement.h
    VertexShader.cpp
    VertexShader.h
    Viewport.cpp
    Viewport.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
