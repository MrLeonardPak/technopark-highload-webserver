cmake_minimum_required(VERSION 3.0)

project(server)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/server.cc
)

include(FetchContent)
FetchContent_Declare (
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${PROJECT_SOURCE_DIR}/inc/server
                            PRIVATE
                            ${PROJECT_SOURCE_DIR}/inc/private
)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
