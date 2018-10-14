if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_subdirectory(DirectX)
else()
    add_subdirectory(XWindow)
endif()

add_subdirectory(OpenGL)
