
file(GLOB srcs *.cpp *.cc)
add_library (common STATIC ${srcs})
target_link_libraries(common crypto protoc protobuf protobuf-lite hiredis_vip)
set_target_properties(common PROPERTIES OUTPUT_NAME "common")
