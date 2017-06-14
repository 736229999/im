g++ -Wall -w -g  -I./ -I../../common -I../../include -lpthread -lrt -lstdc++ -fpic -shared -o libMiniAppNetwork.so -L.  ZAppNetwork.cpp ../../common/SyncCritical.cpp ../../common/ZSignalQueue.cpp ../../common/SyncEvent.cpp ../../common/WinCommonForLinux.cpp ../../common/Utility.cpp ../../common/Log.cpp ../../common/Function.cpp ../../common/ConfigInfo.cpp ../../common/IniFile.cpp  ./MiniAppNetwork.cpp

cp -p ./libMiniAppNetwork.so ../../lib
mv ./libMiniAppNetwork.so ../../bin

