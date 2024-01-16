cmake_minimum_required(VERSION 3.12)
project(Updater VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()

# Recursively add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(updater ${SOURCES})

# Link libraries
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(yaml-cpp REQUIRED)

# this one requires some weird shenanigans
find_package(libzip REQUIRED)

get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

# Probably useless
include_directories(updater PRIVATE include)

include_directories(${YAML_CPP_INCLUDE_DIRS})
include_directories(${ZIP_INCLUDE_DIRS})

# Link
target_link_libraries(updater PRIVATE CURL::libcurl)
target_link_libraries(updater PRIVATE jsoncpp_lib)
target_link_libraries(updater PRIVATE ${YAML_CPP_LIBRARIES})
target_link_libraries(updater PRIVATE zip)
