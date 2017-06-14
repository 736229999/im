file(GLOB srcs ./*.cpp)
IF(var_access)
add_executable (AccessServer ${srcs}) 
set_target_properties(AccessServer PROPERTIES OUTPUT_NAME "AccessServer")
target_link_libraries(AccessServer common MiniNetTrans MiniAppNetwork DataBaseLib
 pthread rt clntsh json curl ssdb-client memcached hessian hessianorb
event zookeeper_mt zookeeper leveldb occi sqlplus rdkafka++ rdkafka ssl sasl2
stdc++ z crypto zmq)
add_dependencies(AccessServer common MiniNetTrans MiniAppNetwork DataBaseLib
 hessian)
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
