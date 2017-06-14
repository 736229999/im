
file(GLOB srcs *.cpp *.cc)
add_library (common STATIC ${srcs})
target_link_libraries(common protoc protobuf protobuf-lite)
set_target_properties(common PROPERTIES OUTPUT_NAME "common")
