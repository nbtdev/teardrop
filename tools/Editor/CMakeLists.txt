set(TARGET Editor)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    MaterialEditor/EditorCanvas.cpp
    MaterialEditor/EditorCanvas.h
    MaterialEditor/EditorCanvasItem.h
    MaterialEditor/ExpressionConnection.cpp
    MaterialEditor/ExpressionConnection.h
    MaterialEditor/ExpressionConnector.cpp
    MaterialEditor/ExpressionConnector.h
    MaterialEditor/ExpressionConnectorDatabase.cpp
    MaterialEditor/ExpressionConnectorDatabase.h
    MaterialEditor/ExpressionItem.cpp
    MaterialEditor/ExpressionItem.h
    MaterialEditor/FauxConnector.cpp
    MaterialEditor/FauxConnector.h
    MaterialEditor/MaterialEditor.cpp
    MaterialEditor/MaterialEditor.h

    ObjectBrowser/IconViewDelegate.cpp
    ObjectBrowser/IconViewDelegate.h
    ObjectBrowser/ListViewDelegate.cpp
    ObjectBrowser/ListViewDelegate.h
    ObjectBrowser/ObjectBrowser.cpp
    ObjectBrowser/ObjectBrowser.h
    ObjectBrowser/ObjectBrowserModel.cpp
    ObjectBrowser/ObjectBrowserModel.h

    ProjectExplorer/ProjectExplorer.cpp
    ProjectExplorer/ProjectExplorer.h
    ProjectExplorer/ProjectItem.cpp
    ProjectExplorer/ProjectItem.h
    ProjectExplorer/ProjectModel.cpp
    ProjectExplorer/ProjectModel.h

    PropertyGrid/PropertyGrid.cpp
    PropertyGrid/PropertyGrid.h
    PropertyGrid/PropertyGridDelegate.cpp
    PropertyGrid/PropertyGridDelegate.h
    PropertyGrid/PropertyGridItem.cpp
    PropertyGrid/PropertyGridItem.h
    PropertyGrid/PropertyGridModel.cpp
    PropertyGrid/PropertyGridModel.h

    Viewers/ObjectViewer3D.cpp
    Viewers/ObjectViewer3D.h
    Viewers/ObjectViewer3DLogic.h
    Viewers/StaticMeshViewer.cpp
    Viewers/StaticMeshViewer.h

    DragDropData.h
    Editor.h
    FolderDragDropData.h
    ObjectDragDropData.h
    Preferences.h
    ProgressDialog.h
    ProgressFeedback.h
    Project.h
    RenderWindow.h
    TypeChooser.h

    DragDropData.cpp
    Editor.cpp
    FolderDragDropData.cpp
    main.cpp
    ObjectDragDropData.cpp
    Preferences.cpp
    ProgressDialog.cpp
    ProgressFeedback.cpp
    Project.cpp
    RenderWindow.cpp
    TypeChooser.cpp
)

find_package(Qt5Widgets CONFIG REQUIRED
    PATHS ${QT5_DIR}
)

set(QTLIBS
    Qt5::Widgets
)

get_target_property(QT5_LIB_DIR Qt5::Core LOCATION)
get_filename_component(QT5_LIB_DIR ${QT5_LIB_DIR} DIRECTORY)

set(QT_INSTALL_LIBS
    $<TARGET_FILE:Qt5::Core>
    $<TARGET_FILE:Qt5::Gui>
    $<TARGET_FILE:Qt5::Widgets>

    $<TARGET_SONAME_FILE:Qt5::Core>
    $<TARGET_SONAME_FILE:Qt5::Gui>
    $<TARGET_SONAME_FILE:Qt5::Widgets>

    ${QT5_LIB_DIR}/libicui18n.so.56
    ${QT5_LIB_DIR}/libicui18n.so.56.1
    ${QT5_LIB_DIR}/libicuuc.so.56
    ${QT5_LIB_DIR}/libicuuc.so.56.1
    ${QT5_LIB_DIR}/libicudata.so.56
    ${QT5_LIB_DIR}/libicudata.so.56.1
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

td_add_executable(${TARGET}
    SOURCES
        ${SOURCES}
)

SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Integrations used on all supported platforms
set(INTEGRATIONS
    Vulkan
    OpenGL
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND INTEGRATIONS
        Direct3D11
    )
else()
    list(APPEND INTEGRATIONS
        XWindow
    )
endif()

target_link_libraries(${TARGET}
    PRIVATE
        PackageManager
        Game
        Math
        tinyxml
        ${QTLIBS}

        # integrations
        ${INTEGRATIONS}
)

#
# Install
#

install(TARGETS
    ${TARGET}
    RUNTIME DESTINATION ${TEARDROP_INSTALL_DIR}/bin

)

install(FILES
    ${QT_INSTALL_LIBS} DESTINATION ${TEARDROP_INSTALL_DIR}/bin
)
