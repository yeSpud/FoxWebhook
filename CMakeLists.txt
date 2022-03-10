cmake_minimum_required(VERSION 3.5)
project(FoxWebhook)

set(CMAKE_CXX_STANDARD 17)

# TumblrAPI
include(FetchContent)
FetchContent_Declare(TumblrAPI GIT_REPOSITORY https://github.com/yeSpud/Tumblr-cpp.git GIT_TAG origin/main)
FetchContent_MakeAvailable(TumblrAPI)

# Add spdlog stuff
find_package(spdlog REQUIRED)

FILE(GLOB MyCPPSources src/*.cpp)
FILE(GLOB MyHPPSources src/*.hpp)

add_executable(FoxWebhook main.cpp ${MyCPPSources} ${MyHPPSources})
target_link_libraries(FoxWebhook PRIVATE TumblrAPI::TumblrAPI cpr::cpr spdlog::spdlog)

add_executable(GetNumberOfPostsUtil get_number_of_posts_util.cpp ${MyCPPSources} ${MyHPPSources})
target_link_libraries(GetNumberOfPostsUtil PRIVATE TumblrAPI::TumblrAPI cpr::cpr spdlog::spdlog)