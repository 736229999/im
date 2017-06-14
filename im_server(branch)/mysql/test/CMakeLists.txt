
include_directories(.)
link_directories(/usr/lib64/mysql)
file(GLOB srcs *.cpp)
add_executable (mysqltest ${srcs})
set_target_properties(mysqltest PROPERTIES OUTPUT_NAME "mysqltest")
target_link_libraries(mysqltest mysqlutil mysqlclient mysqlpp pthread)
add_dependencies(mysqltest mysqlutil)

