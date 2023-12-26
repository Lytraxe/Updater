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

# Probably useless
target_include_directories(updater PRIVATE include)

# Link
target_link_libraries(updater PRIVATE CURL::libcurl)
target_link_libraries(updater PRIVATE yaml-cpp)
target_link_libraries(updater PRIVATE jsoncpp)
target_link_libraries(updater PRIVATE zip)
