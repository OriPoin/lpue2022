cmake_minimum_required(VERSION 3.0.0)
project(copy VERSION 0.1.0)

# boost.program_options & boost.log & boost.test dependency
find_package(Boost REQUIRED COMPONENTS program_options log_setup log unit_test_framework date_time)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

# Verbose GNU Make
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_GENERATOR "Unix Makefiles")

# Set default build type
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__DEBUG__)
endif ()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -pipe -fgraphite-identity -floop-nest-optimize -fno-plt -fno-semantic-interposition -fipa-pta -fno-math-errno -flto=auto -fdevirtualize-at-ltrans")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -pipe -fgraphite-identity -floop-nest-optimize -fno-plt -fno-semantic-interposition -fipa-pta -fno-math-errno -flto=auto -fdevirtualize-at-ltrans")
endif()


# include dir
include_directories("include")
add_subdirectory(include)
# source code dir
add_subdirectory(src)
# tests code tests dir
add_subdirectory(tests)
