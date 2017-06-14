include_directories(aaa)
include_directories(hessian)
include_directories(.)
file(GLOB srcs *.cpp)
add_library (hessian STATIC ${srcs})
set_target_properties(hessian PROPERTIES OUTPUT_NAME "hessian")

