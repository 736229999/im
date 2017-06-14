g++ -Wall -w -g  -I./ -I../../common -I../../include -lpthread -lrt -lstdc++ -L../../lib -ljson -o UserOnlineManager  -L../../lib -L$ORACLE_HOME/lib -lclntsh -lMiniNetTrans -lMiniAppNetwork -lDataBaseLib ../../common/des.cpp ../../common/SyncCritical.cpp ../../common/ZSignalQueue.cpp ../../common/SyncEvent.cpp ../../common/WinCommonForLinux.cpp ../../common/Utility.cpp ../../common/Log.cpp ../../common/Function.cpp ../../common/ConfigInfo.cpp ../../common/IniFile.cpp stdafx.cpp ./OnlineUser.cpp ./BusinessDelOnlineUser.cpp ../../common/BusinessExGSrv.cpp ../../common/Business.cpp ../../common/RouterBusiness.cpp ../../common/RouterProtocol.cpp ../../common/PacketBuffer.cpp  ../../common/SyncSemaphore.cpp ./BusinessAddModifyOnlineUser.cpp ../../common/xtpacket.cpp ./AppManager.cpp ./BusinessGetUserFullInfo.cpp ./BusinessGetUserLinkInfo.cpp ./BusinessGetUserStatus.cpp ./BusinessImServerShutdown.cpp ./BusinessModifyUserStatus.cpp ./BusinessNotifyRelogin.cpp ./BusinessSearchOnlineUser.cpp ./ThreadManager.cpp ./OnlineManager.cpp  ./Mini139OnlineManager.cpp 
mv ./UserOnlineManager ../../bin




