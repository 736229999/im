g++ -Wall -w -g  -I./ -I../../common -I../../include -I/usr/include/oracle/11.2/client64 -L/usr/lib/oracle/11.2/client64/lib -L/home/amin/iecim/bin -lclntsh -lpthread -lrt -lstdc++ -fpic -shared -o libDataBaseLib.so -L. DatabaseGW.cpp ../../common/SyncCritical.cpp ../../common/Log.cpp ../../common/WinCommonForLinux.cpp DatabaseWeb.cpp ../../common/ErrorCode.cpp DatabaseMini.cpp DatabaseFamily.cpp DataBaseLib.cpp
cp -p ./libDataBaseLib.so ../../lib
mv ./libDataBaseLib.so ../../bin

