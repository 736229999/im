
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
file(GLOB srcs ./*.cpp)
add_executable (WebMsgNotify ${srcs})
set_target_properties(WebMsgNotify PROPERTIES OUTPUT_NAME "WebMsgNotify")
target_link_libraries(WebMsgNotify common MiniNetTrans MiniAppNetwork
	DataBaseLib pthread clntsh memcached rt)
add_dependencies(WebMsgNotify common MiniNetTrans MiniAppNetwork DataBaseLib)
