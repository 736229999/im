

file(GLOB srcs ./*.cpp)
add_executable (ImTransfer ${srcs})
set_target_properties(ImTransfer PROPERTIES OUTPUT_NAME "ImTransfer")
target_link_libraries(ImTransfer common MiniNetTrans MiniAppNetwork pthread rt clntsh)
add_dependencies(ImTransfer common MiniNetTrans MiniAppNetwork)

