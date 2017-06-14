cd ../build;cmake .. -Dvar_access:STRING=OFF -Dxmpp_build:STRING=OFF;make -j 2
cmake .. -Dvar_access:STRING=ON -Dxmpp_build:STRING=OFF;make -j 2;cd -
