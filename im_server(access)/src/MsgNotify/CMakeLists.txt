file(GLOB srcs ./*.cpp)
add_executable (MsgNotify ${srcs})
set_target_properties(MsgNotify PROPERTIES OUTPUT_NAME "MsgNotify")
target_link_libraries(MsgNotify common pthread rdkafka++ rdkafka pthread rt ssl sasl2
stdc++ z crypto json curl zmq zookeeper_mt jansson)
add_dependencies(MsgNotify common)