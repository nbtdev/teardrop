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
            ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
endfunction()
