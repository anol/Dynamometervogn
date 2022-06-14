cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)

if ("${Target}" STREQUAL "")

    message(" <> Target is not defined <> ")

else ()

    project(${Target})
    add_subdirectory(${Target})

endif ()
