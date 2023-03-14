cmake_minimum_required(VERSION 3.0)

project(server)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/server.cc
    src/http.cc
    src/config.cc
    src/file.cc
    src/thread.cc
    src/handler.cc
)
# include(FetchContent)
# FetchContent_Declare (
#     spdlog
#     GIT_REPOSITORY https://github.com/gabime/spdlog.git
#     GIT_TAG v1.11.0
# )
# FetchContent_MakeAvailable(spdlog)
find_package(spdlog REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${PROJECT_SOURCE_DIR}/inc
)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog Threads::Threads)
