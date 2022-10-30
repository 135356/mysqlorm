set(mysql_FOUND TRUE)
#项目名称
set(PACKAGE_FIND_NAME mysql)
#头文件路径
set(mysql_INCLUDES /usr/local/mysql/include)
#库文件路径
find_library(mysql_LIBS NAMES mysqlclient PATHS /usr/local/mysql/lib)