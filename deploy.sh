rm packages/win32.zip
cp fluorescence_win32.exe packages/win32

zip -9 packages/win32.zip shards/localhost/* fonts/* gumps/* shader/* themes/default/* themes/default/Images/* data/*
cd packages/win32
zip -9 ../win32.zip *

