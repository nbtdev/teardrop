# build TinyXML target
set(TINYXML_SOURCES
    tinyxml/tinystr.cpp
    tinyxml/tinystr.h
    tinyxml/tinyxml.cpp
    tinyxml/tinyxml.h
    tinyxml/tinyxmlerror.cpp
    tinyxml/tinyxmlparser.cpp
)

add_library(tinyxml STATIC ${TINYXML_SOURCES})

target_include_directories(tinyxml
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
