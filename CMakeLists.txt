get_filename_component(project_name_ ${CMAKE_CURRENT_SOURCE_DIR} NAME)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
project(${project_name_} VERSION 6.0.0)

#Debug｜Release模式: -g表示带调试信息，-Wall表示输出所有的警告信息
set(CMAKE_BUILD_TYPE Release) #cmake根据编译器加-g参数(调试)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILE)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(install_dir /usr/local/bb)
    add_library(mysql_orm SHARED ${SRC_FILE})
    #install(FILES ${CMAKE_BINARY_DIR}/cmake/modules/mysqlConfig.cmake DESTINATION /usr/local/bb/cmake/modules)
else()
    set(install_dir ${CMAKE_SOURCE_DIR}/build)
    add_library(mysql_orm STATIC ${SRC_FILE})
endif()
set(CMAKE_PREFIX_PATH ${install_dir}/cmake/modules)
find_package(bb 6.0 REQUIRED NO_MODULE)
find_package(mysql REQUIRED NO_MODULE)
include_directories(${bb_INCLUDES} ${mysql_INCLUDES})
target_include_directories(mysql_orm PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
#生成可执行文件
add_executable(${PROJECT_NAME}_run main.cpp)
add_dependencies(${PROJECT_NAME}_run mysql_orm)
target_link_libraries(${PROJECT_NAME}_run -lpthread bb mysql_orm ${mysql_LIBS})
set(CMAKE_INSTALL_PREFIX ${install_dir}/${PROJECT_NAME})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/mysql_orm" TYPE INCLUDE)
install(TARGETS mysql_orm
    EXPORT Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(EXPORT Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION cmake
)

set(CMAKE cmake)
set(INCLUDES include)
set(LIBS lib)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    Config.cmake.in
    ${install_dir}/cmake/modules/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${install_dir}/cmake/modules
    PATH_VARS CMAKE INCLUDES LIBS
)
write_basic_package_version_file(
    ${install_dir}/cmake/modules/${PROJECT_NAME}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)