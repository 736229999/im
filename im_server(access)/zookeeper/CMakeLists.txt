include_directories(aaa)
include_directories(zookeeper)
include_directories(.)
file(GLOB srcs *.cpp)
add_library (zookeeper STATIC ${srcs})
set_target_properties(zookeeper PROPERTIES OUTPUT_NAME "zookeeper")

