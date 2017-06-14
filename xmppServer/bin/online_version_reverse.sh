ConfigFile='./Config/Config_online_reverse.ini'
svn diff $ConfigFile

sh imkill 

rm -rf ./bin
mkdir bin

echo $ConfigFile

cp ImServer ./bin

cp libcommon.a libmysqlutil.a ./bin

cp libMiniNetTrans.so libMiniAppNetwork.so libDataBaseLib.so ./bin

cp ../lib/libjson.so ../lib/libmysqlclient.so ../lib/libmysqlpp.so.3 ./bin
cp imstart_1 imstop_1 imlps_1 immonitor_1 imkill ./bin


mkdir ./bin/Config
cp ./Config/ServerList.ini ./bin/Config/ServerList.ini
cp  $ConfigFile ./bin/Config/Config.ini
cp ./Config/webmsg.ini ./bin/Config/webmsg.ini

Day=`date +%Y%m%d`
echo $Day
tar -cvf version_online_reverse_$Day.tar ./bin

rm -rf ./bin
