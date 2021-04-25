cmake_minimum_required(VERSION 3.19)
project(FoxWebhook)

set(CMAKE_CXX_STANDARD 20 -lcurl)

include(ExternalProject)
include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")
#include("${CMAKE_SOURCE_DIR}/vendor/cpr.cmake")
include(FetchContent)
set(CPR_BUILD_TESTS_SSL OFF)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG origin/master)
FetchContent_MakeAvailable(cpr)

include_directories(${RAPIDJSON_INCLUDE_DIR})

#add_executable(FoxWebhook main.cpp src/FoxWebhook.cpp src/FoxWebhook.hpp src/Tumblr/TumblrAPI.cpp src/Tumblr/TumblrAPI.hpp src/DiscordWebhook.cpp src/DiscordWebhook.hpp)
FILE(GLOB MyCPPSources src/*.cpp src/Tumblr/*.cpp)
FILE(GLOB MyHPPSources src/*.hpp src/Tumblr/*.hpp)
add_executable(FoxWebhook main.cpp ${MyCPPSources} ${MyHPPSources})
add_dependencies(FoxWebhook rapidjson)
target_link_libraries(FoxWebhook PRIVATE cpr::cpr)