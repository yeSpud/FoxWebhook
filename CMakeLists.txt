cmake_minimum_required(VERSION 3.15)
project(FoxWebhook)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# cpr
cmake_policy(SET CMP0079 NEW)
if (WIN32)
    set(DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(CPR_FORCE_WINSSL_BACKEND ON)
    set(DCPR_BUILD_TESTS_SSL ON)
endif (WIN32)
set(CPR_BUILD_TESTS OFF)
set(CPR_BUILD_TESTS_SSL OFF)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG db351ffbbadc6c4e9239daaa26e9aefa9f0ec82d)
FetchContent_MakeAvailable(cpr)

# Add spdlog stuff
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.11.0)
FetchContent_MakeAvailable(spdlog)

include_directories(./include)
FILE(GLOB HFILES include/*.*)
FILE(GLOB CPPFILES src/*.cpp)

add_executable(FoxWebhook main.cpp ${CPPFILES} ${HFILES})
target_link_libraries(FoxWebhook PRIVATE cpr::cpr spdlog::spdlog)

add_executable(cron cron.cpp ${CPPFILES} ${HFILES})
target_link_libraries(cron PRIVATE cpr::cpr spdlog::spdlog)

add_executable(GetNumberOfPostsUtil get_number_of_posts_util.cpp ${CPPFILES} ${HFILES})
target_link_libraries(GetNumberOfPostsUtil PRIVATE cpr::cpr spdlog::spdlog)