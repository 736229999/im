cd `dirname $0`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORACLE_HOME/lib
proc parse=none  code=cpp threads=true SQLCHECK=SEMANTICS  "./DatabaseWeb.pc" config=./pcscfg.cfg
rm -rf ./DatabaseWeb.cpp
mv ./DatabaseWeb.c ./DatabaseWeb.cpp
proc parse=none  code=cpp threads=true SQLCHECK=SEMANTICS  "./DatabaseGW.pc" config=./pcscfg.cfg
rm -rf ./DatabaseGW.cpp
mv ./DatabaseGW.c ./DatabaseGW.cpp
proc parse=none  code=cpp threads=true SQLCHECK=SEMANTICS  "./DatabaseMini.pc" config=./pcscfg.cfg
rm -rf ./DatabaseMini.cpp
mv ./DatabaseMini.c ./DatabaseMini.cpp
proc parse=none  code=cpp threads=true SQLCHECK=SEMANTICS  "./DatabaseFamily.pc" config=./pcscfg.cfg
rm -rf ./DatabaseFamily.cpp
mv ./DatabaseFamily.c ./DatabaseFamily.cpp
rm -f tp* *.lis

