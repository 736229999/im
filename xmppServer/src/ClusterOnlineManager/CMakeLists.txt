
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
file(GLOB srcs ./*.cpp)
add_executable (ClusterOnlineManager ${srcs})
set_target_properties(ClusterOnlineManager PROPERTIES OUTPUT_NAME "ClusterOnlineManager")
target_link_libraries(ClusterOnlineManager common MiniNetTrans MiniAppNetwork pthread rt json)
add_dependencies(ClusterOnlineManager common MiniNetTrans MiniAppNetwork)

