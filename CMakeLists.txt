cmake_minimum_required(VERSION 3.19)
project(FoxWebhook)

set(CMAKE_CXX_STANDARD 20 -lcurl)

include(ExternalProject)
include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")
#include("${CMAKE_SOURCE_DIR}/vendor/cpr.cmake")
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG origin/master)
FetchContent_MakeAvailable(cpr)

include_directories(${RAPIDJSON_INCLUDE_DIR})

add_executable(FoxWebhook main.cpp src/FoxWebhook.cpp src/FoxWebhook.hpp src/TumblrAPI.cpp src/TumblrAPI.hpp src/DiscordWebhook.cpp src/DiscordWebhook.hpp)
add_dependencies(FoxWebhook rapidjson)
target_link_libraries(FoxWebhook PRIVATE cpr::cpr)