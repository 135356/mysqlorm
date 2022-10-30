
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../mysql_orm" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

#用于检查路径是否合法
set_and_check(mysql_orm_INCLUDES "${PACKAGE_PREFIX_DIR}/include")
set_and_check(mysql_orm_LIBS "${PACKAGE_PREFIX_DIR}/lib")
set_and_check(mysql_orm_CMAKE "${PACKAGE_PREFIX_DIR}/cmake")

#引入安装生成信息
include("${PACKAGE_PREFIX_DIR}/cmake/mysql_ormTargets.cmake")

#用于检查依赖项是否存在
check_required_components(mysql_orm)
