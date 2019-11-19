find_package(Vulkan)

# library name
set(TARGET Vulkan)

# library source files
set(SOURCES
    CommandBufferVulkan.cpp
    CommandBufferVulkan.h
    CommandQueueVulkan.cpp
    CommandQueueVulkan.h
    IndexBufferVulkan.cpp
    IndexBufferVulkan.h
    IntegrationVulkan.cpp
    IntegrationVulkan.h
    RendererVulkan.cpp
    RendererVulkan.h
    RenderPassVulkan.cpp
    RenderPassVulkan.h
    RenderTargetVulkan.cpp
    RenderTargetVulkan.h
    RenderWindowVulkan.cpp
    RenderWindowVulkan.h
    SynchronizationPrimitiveVulkan.cpp
    SynchronizationPrimitiveVulkan.h
    VertexBufferVulkan.cpp
    VertexBufferVulkan.h
    ViewportVulkan.cpp
    ViewportVulkan.h
)

if(Vulkan_FOUND)
    message(STATUS "Building with Vulkan support: ${Vulkan_LIBRARIES}")
    # create static library with standard build settings
    td_add_library(${TARGET}
        LIBRARY_TYPE
            STATIC
        SOURCES
            ${SOURCES}
    )

    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        list(APPEND PLATFORM_DEPENDENCIES
            X11-xcb
        )
    endif()

    target_link_libraries(${TARGET}
        PUBLIC
            Gfx
            Vulkan::Vulkan
            ${PLATFORM_DEPENDENCIES}
    )
else()
    message(STATUS "Vulkan devel files not found, building without Vulkan support")
endif()
