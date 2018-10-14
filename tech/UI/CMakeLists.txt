# library name
set(TARGET UI)

# library source files
set(SOURCES
    UIComposer.cpp
    UIComposer.h
    UIDefs.h
    UIElement.cpp
    UIElement.h
    UILayer.cpp
    UILayer.h
    UILayoutTarget.h
    UIRenderer.cpp
    UIRenderer.h
    UIUtil.cpp
    UIUtil.h

    #integration/Basic/UIQuadElement.cpp
    integration/Basic/UIQuadElement.h
    #integration/Basic/UIQuadRenderer.cpp
    integration/Basic/UIQuadRenderer.h

    #integration/SWF/UIFlashElement.cpp
    integration/SWF/UIFlashElement.h
    #integration/SWF/UIFlashEvents.cpp
    integration/SWF/UIFlashEvents.h
    #integration/SWF/UIFlashInstance.cpp
    integration/SWF/UIFlashInstance.h
    integration/SWF/UIFlashInterfaces.h
    #integration/SWF/UIFlashRenderer.cpp
    integration/SWF/UIFlashRenderer.h
    #integration/SWF/UIFlashResource.cpp
    integration/SWF/UIFlashResource.h
    #integration/SWF/UIFlashSite.cpp
    integration/SWF/UIFlashSite.h
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

    PRIVATE
        tinyxml
)

target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/integration
)
