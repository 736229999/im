add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DCONFIG_DIR=\"/usr/local/etc\")
add_definitions(-DLIBRARY_DIR=\"/usr/local/lib/jabberd\")

add_subdirectory(c2s)
add_subdirectory(sx)
add_subdirectory(mio)
