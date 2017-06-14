

file(GLOB srcs ./*.cpp)
add_executable (UserOnlineManager ${srcs})
set_target_properties(UserOnlineManager PROPERTIES OUTPUT_NAME "UserOnlineManager")
target_link_libraries(UserOnlineManager common MiniNetTrans MiniAppNetwork DataBaseLib pthread rt clntsh json)
add_dependencies(UserOnlineManager common MiniNetTrans MiniAppNetwork DataBaseLib)

