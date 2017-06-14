file(GLOB srcs ./*.cpp)
IF(var_access)
add_executable (AccessServer ${srcs}) 
IF(xmpp_build)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DCONFIG_DIR=\"/usr/local/etc\")
ENDIF(xmpp_build)
set_target_properties(AccessServer PROPERTIES OUTPUT_NAME "AccessServer")
IF(xmpp_build)
target_link_libraries(AccessServer common MiniNetTrans MiniAppNetwork_Access DataBaseLib
 pthread rt clntsh json curl ssdb-client memcached hessian hessianorb
event zookeeper_mt zookeeper leveldb occi sqlplus rdkafka++ rdkafka ssl sasl2
stdc++ z crypto zmq c2s gsasl udns idn expat resolv crypt dl mio subst sx util)
add_dependencies(AccessServer common MiniNetTrans MiniAppNetwork_Access DataBaseLib
 hessian jabberd)
ELSE()
target_link_libraries(AccessServer common MiniNetTrans MiniAppNetwork_Access DataBaseLib
 pthread rt clntsh json curl ssdb-client memcached hessian hessianorb
event zookeeper_mt zookeeper leveldb occi sqlplus rdkafka++ rdkafka ssl sasl2
stdc++ z crypto zmq)
add_dependencies(AccessServer common MiniNetTrans MiniAppNetwork_Access DataBaseLib
 hessian)
ENDIF(xmpp_build)
ELSE()
add_executable (ImServer ${srcs}) 
set_target_properties(ImServer PROPERTIES OUTPUT_NAME "ImServer")
target_link_libraries(ImServer common MiniNetTrans MiniAppNetwork DataBaseLib
 pthread rt clntsh json curl ssdb-client memcached hessian hessianorb
event zookeeper_mt zookeeper leveldb occi sqlplus rdkafka++ rdkafka ssl sasl2
stdc++ z crypto zmq)
add_dependencies(ImServer common MiniNetTrans MiniAppNetwork DataBaseLib
 hessian)
ENDIF()
