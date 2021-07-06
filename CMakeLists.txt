cmake_minimum_required(VERSION 3.16)
project(FoxWebhook)

set(CMAKE_CXX_STANDARD 20)

# Rapidjson stuff
include(ExternalProject)
include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")
include_directories(${RAPIDJSON_INCLUDE_DIR})

# CPR stuff
include(FetchContent)
if(WIN32)
	set(DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(CPR_FORCE_WINSSL_BACKEND ON)
	set(DCPR_BUILD_TESTS_SSL ON)
endif(WIN32)
set(CPR_BUILD_TESTS OFF)
set(CPR_BUILD_TESTS_SSL OFF)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG origin/master)
FetchContent_MakeAvailable(cpr)

# Spdlog stuff
add_subdirectory(vendor/spdlog)
find_package(spdlog)

#add_executable(FoxWebhook main.cpp src/Tumblr/Blog.cpp src/Tumblr/Blog.hpp src/Tumblr/Image.cpp src/Tumblr/Image.hpp src/Tumblr/Post.cpp src/Tumblr/Post.hpp src/Tumblr/TumblrAPI.hpp src/DiscordWebhook.hpp src/DiscordWebhook.cpp src/FoxWebhook.cpp src/FoxWebhook.hpp)
FILE(GLOB MyCPPSources src/*.cpp src/Tumblr/*.cpp)
FILE(GLOB MyHPPSources src/*.hpp src/Tumblr/*.hpp)
add_executable(FoxWebhook main.cpp ${MyCPPSources} ${MyHPPSources})
#add_executable(FoxWebhook main.cpp)
add_dependencies(FoxWebhook rapidjson)
target_link_libraries(FoxWebhook PRIVATE cpr::cpr spdlog)