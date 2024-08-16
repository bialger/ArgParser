cmake_minimum_required(VERSION 3.12)

project(
        ArgParser
        VERSION 1.2.5
        DESCRIPTION "C++ CLI argument parser library"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "Building ${PROJECT_DESCRIPTION} ${PROJECT_NAME}, version ${PROJECT_VERSION}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

add_subdirectory(lib)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(bin)

    enable_testing()
    add_subdirectory(tests)
endif ()
