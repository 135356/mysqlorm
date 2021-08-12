cmake_minimum_required(VERSION 3.17)
project(BB_DB)

set(CMAKE_CXX_STANDARD 14)

link_directories("./cmake-build-debug/lib")
file(GLOB CPP_FILE src/*/*.cpp)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(BB_DB STATIC ${CPP_FILE})
add_executable(BB_DB_main main.cpp)

include_directories("include")
target_link_libraries(BB_DB_main mysqlclient)
target_link_libraries(BB_DB_main hiredis)
target_link_libraries(BB_DB_main bb)
target_link_libraries(BB_DB_main BB_DB)