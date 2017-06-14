#required cmake version
cmake_minimum_required(VERSION 2.6)

IF(var_access)
add_definitions(-DACCESS_SERVER)
ELSE()
ENDIF()

#project name
project(ImServer)

#version no.
set(MAJORVERSION 1)
set(MINORVERSION 1)

#versions
set(BUILDTYPE Alpha)  

#build release version, to do
OPTION(RELEASED "build this project for release" OFF)

#using static libs
OPTION(USE_STATIC_LIB "use static lib" ON)

#using cache in proxy
OPTION(USE_PROXY_CACHE "use proxy cache" ON)

#broadcast log whether use yfs
OPTION(BROADCAST_USE_YFS "use yfs as broadcast storage" ON)

#system whether use lease
OPTION(USE_LEASE "use lease at write opreation" OFF)



#DEBUG
OPTION(DEBUG "use lease at write opreation" ON)

IF(xmpp_build)
#use XMPP_BUILD 
message("use XMPP_BUILD")
add_definitions(-DXMPP_BUILD)
ELSE()
ENDIF()


EXEC_PROGRAM(
svn
${PROJECT_SOURCE_DIR}
ARGS info | awk '/^版本:|Revision:/{print $2}'                                             
OUTPUT_VARIABLE BUILDON                                                                  
RETURN_VALUE SVNRESULT                                                                   
)   
if(SVNRESULT)
    set(BUILDON "0000")                                                      
endif(SVNRESULT)                                                                         

EXEC_PROGRAM(
date 
${PROJECT_SOURCE_DIR}
ARGS +"%F %T"                                                                            
OUTPUT_VARIABLE BUILDTIME                                                                
)

set(BUILDVERSION "${PROJECT_NAME} ${MAJORVERSION}.${MINORVERSION}.${BUILDON} ${BUILDTYPE} build(${BUILDTIME})") 

#reset output path
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


#include dirs
include_directories(. /usr/local/include /usr/include/mysql
${PROJECT_SOURCE_DIR}/include 
${PROJECT_SOURCE_DIR}/common 
IF(xmpp_build)
${PROJECT_SOURCE_DIR}/jabberd 
ENDIF(xmpp_build)
${PROJECT_SOURCE_DIR}/mysql 
${PROJECT_SOURCE_DIR}/hessian /usr/include/oracle/11.2/client64
${PROJECT_SOURCE_DIR}/include/zeromq
${PROJECT_SOURCE_DIR}/zookeeper)
link_directories(${LIBRARY_OUTPUT_PATH} ${PROJECT_SOURCE_DIR}/lib /usr/local/lib /usr/lib/oracle/11.2/client64/lib /usr/lib64/mysql)

add_definitions(-DPOSIX)
#add_definitions(-DTEST_TIME)

#use release version
IF(RELEASED)
    add_definitions(-D_USE_RELEASE)
    set(CMAKE_CXX_FLAGS "-fPIC -O3 -std=c++11")
ELSE(RELEASED)
    set(CMAKE_CXX_FLAGS "-fPIC -g -Wall -std=c++11")
ENDIF(RELEASED)

#use mysql
IF(MYSQL)
	message("use mysql")
	add_definitions(-D_USE_MYSQL)
ENDIF(MYSQL)



#use PRESS_TEST
#message("use PRESS_TEST")
#add_definitions(-DPRESS_TEST)

#use web time statistics
#add_definitions(-DTIME_STATISITC)

#use cached default use memcached
add_definitions(-DIM_USE_CACHE)
#set this means use redis rather than memcached
add_definitions(-DIM_USE_REDIS)
#use zookeeper
#add_definitions(-DIM_USE_ZOOKEEPER)


#use IM_USE_MULTIDEVICE
#message("use IM_USE_MULTIDEVICE")
#add_definitions(-DIM_USE_MULTIDEVICE)

#use PRE_INTERNAL 
#message("use PRE_INTERNAL")
#add_definitions(-DPRE_INTERNAL)


#use debug
IF(DEBUG)
	message("use debug")
	add_definitions(-DDEBUG)
ENDIF(DEBUG)

# warning: no-deprecated
add_definitions(-Wno-deprecated)
#child dir
add_subdirectory(mysql)
add_subdirectory(src)
add_subdirectory(common)
add_subdirectory(hessian)
add_subdirectory(zookeeper)
IF(xmpp_build)
add_subdirectory(jabberd)
ENDIF(xmpp_build)