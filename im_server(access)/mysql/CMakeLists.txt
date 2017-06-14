
include_directories(.)
file(GLOB srcs *.cpp)
add_library (mysqlutil STATIC ${srcs})
set_target_properties(mysqlutil PROPERTIES OUTPUT_NAME "mysqlutil")

add_subdirectory(test)
