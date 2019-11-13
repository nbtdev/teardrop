find_package(Vulkan)

# library name
set(TARGET Vulkan)

# library source files
set(SOURCES
    IntegrationVulkan.cpp
    IntegrationVulkan.h
    RendererVulkan.cpp
    RendererVulkan.h
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

    target_link_libraries(${TARGET}
        PUBLIC
            Gfx
            Vulkan::Vulkan
            ${PLATFORM_DEPENDENCIES}
    )
else()
    message(STATUS "Vulkan devel files not found, building without Vulkan support")
endif()
