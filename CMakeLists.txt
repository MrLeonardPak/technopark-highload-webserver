cmake_minimum_required(VERSION 3.0)

project(webserver-highload)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(server)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE server)
