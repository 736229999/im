gcc -Wall -w -g  -I./ -I../../common -I../../include -lpthread -lrt -lstdc++ -fpic -shared -o libMiniNetTrans.so  ZUdpComm.cpp ../../common/SyncCritical.cpp ../../common/ZSignalQueue.cpp ../../common/SyncEvent.cpp ../../common/WinCommonForLinux.cpp ../../common/Utility.cpp  ZUdpCommExt.cpp ../../common/HashList.cpp MiniNetTrans.cpp ../MiniIM/SysStatic.cpp ../../common/JsonParse.cpp
cp -p ./libMiniNetTrans.so ../../lib
mv ./libMiniNetTrans.so ../../bin

