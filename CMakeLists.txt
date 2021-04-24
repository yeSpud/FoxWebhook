cmake_minimum_required(VERSION 3.19)
project(FoxWebhook)

set(CMAKE_CXX_STANDARD 20)

include(ExternalProject)
include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")


include_directories(${RAPIDJSON_INCLUDE_DIR})

add_executable(FoxWebhook main.cpp src/FoxWebhook.cpp src/FoxWebhook.hpp)
add_dependencies(FoxWebhook rapidjson)