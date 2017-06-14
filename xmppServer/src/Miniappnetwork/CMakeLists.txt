
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
file(GLOB srcs ./*.cpp)
add_library (MiniAppNetwork SHARED ${srcs})
set_target_properties(MiniAppNetwork PROPERTIES OUTPUT_NAME "MiniAppNetwork")
target_link_libraries(MiniAppNetwork common)
add_dependencies(MiniAppNetwork common)
