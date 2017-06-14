

file(GLOB srcs ./*.cpp)
add_executable (ImServer ${srcs})
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DCONFIG_DIR=\"/usr/local/etc\")
set_target_properties(ImServer PROPERTIES OUTPUT_NAME "ImServer")
target_link_libraries(ImServer common MiniNetTrans MiniAppNetwork DataBaseLib
ssdbconn pthread rt clntsh json curl ssdb-client memcached hessian hessianorb
event occi sqlplus c2s ssl crypto gsasl
udns idn expat resolv crypt dl z mio subst sx util)
add_dependencies(ImServer common MiniNetTrans MiniAppNetwork DataBaseLib
ssdbconn hessian jabberd)

