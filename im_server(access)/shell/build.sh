cd ../build;cmake .. -Dvar_access:STRING=OFF;make -j 4
cmake .. -Dvar_access:STRING=ON;make -j 4;cd -
