
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
file(GLOB srcs ./*.cpp)
add_library (MiniNetTrans SHARED ${srcs})
set_target_properties(MiniNetTrans PROPERTIES OUTPUT_NAME "MiniNetTrans")
target_link_libraries(MiniNetTrans common json)
add_dependencies(MiniNetTrans common)
