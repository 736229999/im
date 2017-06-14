ConfigFile='./Config/Config_preview.ini'

svn diff $ConfigFile 

sh imkill 

rm -rf ./bin
mkdir bin

echo $ConfigFile

cp ImServer UserOnlineManager ClusterOnlineManager WebMsgNotify ./bin

cp libcommon.a libmysqlutil.a ./bin

cp libMiniNetTrans.so libMiniAppNetwork.so libDataBaseLib.so ./bin

cp ../lib/libjson.so ../lib/libmysqlclient.so ../lib/libmysqlpp.so.3 ./bin

cp imstart imstop imlps immonitor imkill ./bin


mkdir ./bin/Config
cp ./Config/ServerList.ini ./bin/Config/ServerList.ini
cp $ConfigFile ./bin/Config/Config.ini
cp ./Config/webmsg.ini ./bin/Config/webmsg.ini

Day=`date +%Y%m%d`
echo $Day
tar -cvf version_preview_$Day.tar ./bin

rm -rf ./bin
