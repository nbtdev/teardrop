cmake_minimum_required(VERSION 3.0)

enable_language(CXX)

# allow version info in project
cmake_policy(SET CMP0048 NEW)

# allow IN_LIST operator
cmake_policy(SET CMP0057 NEW)

project(Teardrop
    VERSION 2018.12
)

# include build helpers
include(cmake/td-targets.cmake)

add_subdirectory(tech)
