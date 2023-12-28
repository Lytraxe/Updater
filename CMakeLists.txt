cmake_minimum_required(VERSION 3.12)
project(Updater VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

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
