cmake_minimum_required(VERSION 3.0)

project(webserver-highload)

add_subdirectory(server)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE server)
