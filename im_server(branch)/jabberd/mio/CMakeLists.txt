include_directories(../mio)
include_directories(../sx)
include_directories(../util)
#include_directories(..jabberd)
include_directories(.)
include_directories(../../common)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DCONFIG_DIR=\"/usr/local/etc\")
add_definitions(-DLIBRARY_DIR=\"/usr/local/lib/jabberd\")
file(GLOB srcs  *.c)
add_library (mio STATIC ${srcs})
set_target_properties(mio PROPERTIES OUTPUT_NAME "mio")

