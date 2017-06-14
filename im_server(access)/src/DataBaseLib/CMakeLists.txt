

execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/proc_make64.sh)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
file(GLOB srcs ./*.cpp)
add_library (DataBaseLib SHARED ${srcs})
set_target_properties(DataBaseLib PROPERTIES OUTPUT_NAME "DataBaseLib")
target_link_libraries(DataBaseLib common mysqlutil mysqlpp mysqlclient)
add_dependencies(DataBaseLib common mysqlutil)

#add_custom_command(TARGET DataBaseLib
#PRE_BUILD
#COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/proc_make64.sh)

