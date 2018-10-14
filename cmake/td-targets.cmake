function(td_add_library TD_LIBRARY)
    cmake_parse_arguments(
        # all arguments prefixed with "TD_"
        TD

        # unary arguments
        ""

        # single-valued arguments
        "LIBRARY_TYPE"

        # multi-valued arguments
        "SOURCES"

        # everything else
        ${ARGN}
    )

    add_library(
        ${TD_LIBRARY}
        ${TD_LIBRARY_TYPE}
        ${TD_SOURCES}
    )

    target_include_directories(${TD_LIBRARY}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}

        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

    target_compile_options(${TD_LIBRARY}
        PRIVATE

        PUBLIC
    )
endfunction()

function(td_add_executable TD_EXECUTABLE)
    cmake_parse_arguments(
        # all arguments prefixed with "TD_"
        TD

        # unary arguments
        ""

        # single-valued arguments
        ""

        # multi-valued arguments
        "SOURCES"

        # everything else
        ${ARGN}
    )

    add_executable(
        ${TD_EXECUTABLE}
        ${TD_SOURCES}
    )
endfunction()
