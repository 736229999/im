
include_directories(.)
file(GLOB srcs *.cpp)
add_library (ssdbconn STATIC ${srcs})
set_target_properties(ssdbconn PROPERTIES OUTPUT_NAME "ssdbconn")

