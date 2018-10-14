if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_subdirectory(DirectX)
endif()

add_subdirectory(OpenGL)
