cmake_minimum_required(VERSION 3.21)
project(MysqlOrm)

set(CMAKE_CXX_STANDARD 14)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)

add_executable(main main.cpp)

include_directories("include")
find_library(bb bb ${CMAKE_BINARY_DIR}/build/lib)
find_library(mysqlclient mysqlclient ${CMAKE_BINARY_DIR}/build/lib)
find_library(MysqlOrm MysqlOrm ${CMAKE_BINARY_DIR}/build/lib)
target_link_libraries(main ${bb} ${mysqlclient} ${MysqlOrm})