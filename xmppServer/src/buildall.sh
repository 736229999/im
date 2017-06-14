echo "make Mininettrans ..."
cd Mininettrans
sh build.sh
echo "make Miniappnetwork ..."
cd ../Miniappnetwork
sh build.sh
echo "make DataBaseLib ..."
cd ../DataBaseLib
sh proc_make64.sh
sh build.sh
echo "make MiniIM ..."
cd ../MiniIM
sh build.sh
echo "make Mini139OnlineManager ..."
cd ../Mini139OnlineManager
sh build.sh
echo "make ClusterOnlineManager ..."
cd ../ClusterOnlineManager
sh build.sh
echo "make WebMsgNotify ..."
cd ../MiniWebMsgNotify
sh build.sh
cd ..
echo -e "\n\nall done!\n\n"
ls -ltr ../bin
