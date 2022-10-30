#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mysql_orm" for configuration "Release"
set_property(TARGET mysql_orm APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mysql_orm PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmysql_orm.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS mysql_orm )
list(APPEND _IMPORT_CHECK_FILES_FOR_mysql_orm "${_IMPORT_PREFIX}/lib/libmysql_orm.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
