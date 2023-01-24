cmake_minimum_required(VERSION 3.15)
project(FoxWebhook)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# cpr
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 47438c7c73d61463520b27759ea58ffb03a31cec #db351ffbbadc6c4e9239daaa26e9aefa9f0ec82d
        CMAKE_ARGS
            -DCPR_BUILD_TESTS=OFF
            -DCPR_BUILD_TESTS_SSL=OFF
)
if (WIN32)
    set(DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(CPR_FORCE_WINSSL_BACKEND ON)
endif (WIN32)

# rapidjson
#FetchContent_Declare(
#        rapidjson
#        GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
#        GIT_TAG 012be8528783cdbf4b7a9e64f78bd8f056b97e24
#        CMAKE_ARGS
#            -DRAPIDJSON_BUILD_TESTS=OFF
#            -DRAPIDJSON_BUILD_DOC=OFF
#            -DRAPIDJSON_BUILD_EXAMPLES=OFF
#            -DRAPIDJSON_HAS_STDSTRING=ON
#)
#set(RAPIDJSON_INCLUDE_DIR ${source_dir}/include)
#set(RAPIDJSON_HAS_STDSTRING ON)

# spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
)

FetchContent_MakeAvailable(cpr spdlog)
include_directories(./include)
FILE(GLOB HFILES include/*.*)
FILE(GLOB CPPFILES src/*.cpp)

add_executable(FoxWebhook main.cpp ${CPPFILES} ${HFILES})
target_link_libraries(FoxWebhook PRIVATE cpr::cpr spdlog::spdlog)

add_executable(cron cron.cpp ${CPPFILES} ${HFILES})
target_link_libraries(cron PRIVATE cpr::cpr spdlog::spdlog)

add_executable(GetNumberOfPostsUtil get_number_of_posts_util.cpp ${CPPFILES} ${HFILES})
target_link_libraries(GetNumberOfPostsUtil PRIVATE cpr::cpr spdlog::spdlog)